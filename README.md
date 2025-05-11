# Games101 Experiment 4

## Experiment Content

- This experiment is relatively simple. The goal is to implement a Bézier curve using recursion.
- The main idea is to pass an array of control points to the function, then compute `n - 1` new points from the original `n` control points based on the parameter `t`. The new set of points is then passed recursively into the same function. The recursion stops when there is only one point left in the array.

## Experiment Result

- <img src="https://github.com/corgiInequation/Games101_bezierCurve/blob/main/my_bezier_curve.png" alt="bezier-curve" width="66%" />
- As shown, a Bézier curve is successfully formed using four control points.
