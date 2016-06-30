# Installation

## Setup
* C++ dependencies: OpenCV
* Python dependencies: numpy, matplotlib, OpenCV
* Clone this repository.
* Download GridCut from here: http://www.gridcut.com/
* Create an `include` directory in this repository.
* From gridcut, copy the folder `include/GridCut` to `include`
* Also from gridcut, copy the folder `examples/include/AlphaExpansion` to `include/GridCut`
* Get PyMaxflow with: `sudo pip install PyMaxflow`

## Build
```
mkdir build
cd build
cmake..
make
```

## Run
```
cd python
python binary.py
python grabcut.py ../data/messi5.jpg

cd ../build
./denoise ../data/penguin-input.png ../data/penguin-mask.png 50
./denoise ../data/house-input.png ../data/house-mask.png 20
```

