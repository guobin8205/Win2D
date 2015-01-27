﻿// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may
// not use these files except in compliance with the License. You may obtain
// a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

using System;
using System.Diagnostics;
using System.Numerics;
using Microsoft.Graphics.Canvas;
using Microsoft.Graphics.Canvas.Effects;
using Windows.Foundation;
using Windows.UI;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Input;

namespace ExampleGallery
{
    // Implements Conway's Game of Life using GPU image effects.
    public sealed partial class GameOfLife : UserControl
    {
        const int simulationW = 128;
        const int simulationH = 128;

        // The simulation is updated by swapping back and forth between two surfaces,
        // repeatedly drawing the contents of one onto the other using an image effect
        // that implements the rules of the cellular automaton.
        CanvasRenderTarget currentSurface, nextSurface;

        public bool Slow { get; set; }
        public bool Paused { get; set; }

        Stopwatch timer;
        TimeSpan lastTime;

        bool isPointerDown;
        int lastPointerX, lastPointerY;

        ConvolveMatrixEffect countNeighborsEffect;
        DiscreteTransferEffect liveOrDieEffect;
        LinearTransferEffect invertEffect;
        Transform2DEffect transformEffect;

        public GameOfLife()
        {
            this.InitializeComponent();

            timer = Stopwatch.StartNew();
        }

        void Canvas_CreateResources(CanvasControl sender, object args)
        {
            const float defaultDpi = 96;

            currentSurface = new CanvasRenderTarget(sender, simulationW, simulationH, defaultDpi);
            nextSurface = new CanvasRenderTarget(sender, simulationW, simulationH, defaultDpi);

            CreateEffects();

            // Initialize cells to random values.
            RandomizeSimulation(null, null);
        }

        void Canvas_Draw(CanvasControl sender, CanvasDrawEventArgs args)
        {
            // Update the simulation state.
            if (!Paused)
            {
                var currentTime = timer.Elapsed;
                var elapsed = (currentTime - lastTime).TotalSeconds;

                if (elapsed >= (Slow ? 0.25 : 1f / 30))
                {
                    UpdateSimulation();
                    lastTime = currentTime;
                }
            }

            // Display the current surface.
            invertEffect.Source = currentSurface;
            transformEffect.TransformMatrix = GetDisplayTransform();
            args.DrawingSession.DrawImage(transformEffect);

            sender.Invalidate();
        }

        void UpdateSimulation()
        {
            // Use the current surface as input.
            countNeighborsEffect.Source = currentSurface;

            // Draw it onto the next surface, using an image effect that implements the Game of Life cellular automaton.
            using (var ds = nextSurface.CreateDrawingSession())
            {
                ds.DrawImage(liveOrDieEffect);
            }

            // Swap the current and next surfaces.
            var tmp = currentSurface;
            currentSurface = nextSurface;
            nextSurface = tmp;
        }

        void CreateEffects()
        {
            // The Game of Life is a cellular automaton with very simple rules.
            // Each cell (pixel) can be either alive (white) or dead (black).
            // The state is updated by:
            //
            //  - for each cell, count how many of its 8 neighbors are alive
            //  - if less than two, the cell dies from loneliness
            //  - if exactly two, the cell keeps its current state
            //  - if exactly three, the cell become alive
            //  - if more than three, the cell dies from overcrowding

            // Step 1: use a convolve matrix to count how many neighbors are alive. This filter
            // also includes the state of the current cell, but with a lower weighting. The result
            // is an arithmetic encoding where (value / 2) indicates how many neighbors are alive,
            // and (value % 2) is the state of the cell itself. This is divided by 18 to make it
            // fit within 0-1 color range.

            countNeighborsEffect = new ConvolveMatrixEffect
            {
                KernelMatrix = new float[]
                {
                    2, 2, 2,
                    2, 1, 2,
                    2, 2, 2
                },

                Divisor = 18,
                BorderMode = EffectBorderMode.Hard,
            };

            // Step 2: use a color transfer table to map the different states produced by the
            // convolve matrix to whether the cell should live or die. Each pair of entries in
            // this table corresponds to a certain number of live neighbors. The first of the
            // pair is the result if the current cell is dead, or the second if it is alive.

            float[] transferTable =
            {
                0, 0,   // 0 live neighbors -> dead cell
                0, 0,   // 1 live neighbors -> dead cell
                0, 1,   // 2 live neighbors -> cell keeps its current state
                1, 1,   // 3 live neighbors -> live cell
                0, 0,   // 4 live neighbors -> dead cell
                0, 0,   // 5 live neighbors -> dead cell
                0, 0,   // 6 live neighbors -> dead cell
                0, 0,   // 7 live neighbors -> dead cell
                0, 0,   // 8 live neighbors -> dead cell
            };

            liveOrDieEffect = new DiscreteTransferEffect
            {
                Source = countNeighborsEffect,

                RedTable = transferTable,
                GreenTable = transferTable,
                BlueTable = transferTable,
            };

            // Step 3: the algorithm is implemented in terms of white = live,
            // black = dead, but we invert these colors before displaying the
            // result, just 'cause I think it looks better that way.

            invertEffect = new LinearTransferEffect
            {
                RedSlope   = -1, RedOffset   = 1,
                GreenSlope = -1, GreenOffset = 1,
                BlueSlope  = -1, BlueOffset  = 1,
            };

            // Step 4: insert our own DPI compensation effect to stop the system trying to
            // automatically convert DPI for us. The Game of Life simulation always works
            // in pixels (96 DPI) regardless of display DPI. Normally, the system would
            // handle this mismatch automatically and scale the image up as needed to fit
            // higher DPI displays. We don't want that behavior here, because it would use
            // a linear filter while we want nearest neighbor. So we insert a no-op DPI
            // converter of our own. This overrides the default adjustment by telling the
            // system the source image is already the same DPI as the destination canvas
            // (even though it really isn't). We'll handle any necessary scaling later
            // ourselves, using Transform2DEffect to control the interpolation mode.

            var dpiCompensationEffect = new DpiCompensationEffect
            {
                Source = invertEffect,
                SourceDpi = new Vector2(canvas.Dpi),
            };

            // Step 5: a transform matrix scales up the simulation rendertarget and moves
            // it to the right part of the screen. This uses nearest neighbor filtering
            // to avoid unwanted blurring of the cell shapes.

            transformEffect = new Transform2DEffect
            {
                Source = dpiCompensationEffect,
                InterpolationMode = CanvasImageInterpolation.NearestNeighbor,
            };
        }

        Matrix3x2 GetDisplayTransform()
        {
            // Scale our simulation surface to fill the CanvasControl.
            var canvasSize = new Vector2((float)canvas.ActualWidth, (float)canvas.ActualHeight);
            var simulationSize = new Vector2(canvas.ConvertPixelsToDips(simulationW), canvas.ConvertPixelsToDips(simulationH));
            var scale = canvasSize / simulationSize;
            var offset = Vector2.Zero;

            // Letterbox or pillarbox to preserve aspect ratio.
            if (scale.X > scale.Y)
            {
                scale.X = scale.Y;
                offset.X = (canvasSize.X - simulationSize.X * scale.X) / 2;
            }
            else
            {
                scale.Y = scale.X;
                offset.Y = (canvasSize.Y - simulationSize.Y * scale.Y) / 2;
            }

            return Matrix3x2.CreateScale(scale) *
                   Matrix3x2.CreateTranslation(offset);
        }

        // Initializes the simulation to a random state.
        void RandomizeSimulation(object sender, RoutedEventArgs e)
        {
            Random random = new Random();

            var colors = new Color[simulationW * simulationH];

            for (int i = 0; i < colors.Length; i++)
            {
                colors[i] = (random.NextDouble() < 0.25) ? Colors.White : Colors.Black;
            }

            currentSurface.SetPixelColors(colors);
        }

        // Clears the simulation state.
        void ClearSimulation(object sender, RoutedEventArgs e)
        {
            using (var ds = currentSurface.CreateDrawingSession())
            {
                ds.Clear(Colors.Black);
            }
        }

        void Canvas_PointerPressed(object sender, PointerRoutedEventArgs e)
        {
            isPointerDown = true;
            lastPointerX = lastPointerY = int.MaxValue;

            ProcessPointerInput(e);
        }

        void Canvas_PointerReleased(object sender, PointerRoutedEventArgs e)
        {
            isPointerDown = false;
        }

        void Canvas_PointerMoved(object sender, PointerRoutedEventArgs e)
        {
            ProcessPointerInput(e);
        }

        // Toggles the color of cells when they are clicked on.
        private void ProcessPointerInput(PointerRoutedEventArgs e)
        {
            if (!isPointerDown)
                return;

            // Invert the display transform, to convert pointer positions into simulation rendertarget space.
            Matrix3x2 transform;
            Matrix3x2.Invert(GetDisplayTransform(), out transform);

            foreach (var point in e.GetIntermediatePoints(canvas))
            {
                if (!point.IsInContact)
                    continue;

                var pos = Vector2.Transform(point.Position.ToVector2(), transform);

                var x = canvas.ConvertDipsToPixels(pos.X);
                var y = canvas.ConvertDipsToPixels(pos.Y);

                // If the point is within the bounds of the rendertarget, and not the same as the last point...
                if (x >= 0 && 
                    y >= 0 && 
                    x < simulationW && 
                    y < simulationH && 
                    ((x != lastPointerX || y != lastPointerY)))
                {
                    // Read the current color.
                    var cellColor = currentSurface.GetPixelColors(x, y, 1, 1);

                    // Toggle the value.
                    cellColor[0] = cellColor[0].R > 0 ? Colors.Black : Colors.White;

                    // Set the new color.
                    currentSurface.SetPixelColors(cellColor, x, y, 1, 1);

                    lastPointerX = x;
                    lastPointerY = y;
                }
            }
        }
    }
}
