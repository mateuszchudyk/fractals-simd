#!/bin/bash

WORK_DIR=$(mktemp -d)
echo "Output directory: ${WORK_DIR}"

./demo mandelbrot 3840x2160 --output ${WORK_DIR}/mandelbrot.bmp
./demo burning-ship 3840x2160 --output ${WORK_DIR}/burning-ship.bmp
./demo julia-set 3840x2160 --output ${WORK_DIR}/julia-set.bmp
./demo julia-set 3840x2160 --iterations 10000 --output ${WORK_DIR}/julia-set-10000.bmp

./demo mandelbrot 200x150 --scale 55 --colormap blue --output ${WORK_DIR}/colormap_blue.bmp
./demo mandelbrot 200x150 --scale 55 --colormap cividis --output ${WORK_DIR}/colormap_cividis.bmp
./demo mandelbrot 200x150 --scale 55 --colormap cyan --output ${WORK_DIR}/colormap_cyan.bmp
./demo mandelbrot 200x150 --scale 55 --colormap gray --output ${WORK_DIR}/colormap_gray.bmp
./demo mandelbrot 200x150 --scale 55 --colormap green --output ${WORK_DIR}/colormap_green.bmp
./demo mandelbrot 200x150 --scale 55 --colormap hot --output ${WORK_DIR}/colormap_hot.bmp
./demo mandelbrot 200x150 --scale 55 --colormap jet --output ${WORK_DIR}/colormap_jet.bmp
./demo mandelbrot 200x150 --scale 55 --colormap magenta --output ${WORK_DIR}/colormap_magenta.bmp
./demo mandelbrot 200x150 --scale 55 --colormap red --output ${WORK_DIR}/colormap_red.bmp
./demo mandelbrot 200x150 --scale 55 --colormap turbo --output ${WORK_DIR}/colormap_turbo.bmp
./demo mandelbrot 200x150 --scale 55 --colormap twilight --output ${WORK_DIR}/colormap_twilight.bmp
./demo mandelbrot 200x150 --scale 55 --colormap yellow --output ${WORK_DIR}/colormap_yellow.bmp

for file in ${WORK_DIR}/*.bmp
do
    convert ${file} ${file%.bmp}.png
done

rm -rf ${WORK_DIR}/*.bmp
