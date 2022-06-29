# ASUS DITCamera project

## Introduction
To detect a lens external condition, like dust, stain, illumination, ... ,etc, we build three algorithm to detect specific problem.
#### 1. Shading
- Definition: The light fall-off observed towards the edges of an image.
- Reason: 
	- Lens vignetting
	- The angular dependence of quantum efficiency due to use microlenses.
	-  ... etc.
- Detect environment: White card.
- Detect method
	1. Evaluate highest pixel block ($\frac{1}{10}W\times\frac{1}{10}H$) average $Y_C$ from center block.
	2. Evaluate corner pixel block average $\text{Y}_1,\text{Y}_2,\text{Y}_3,\text{Y}_4$  ($\frac{1}{10}\text{W}\times\frac{1}{10}\text{H}$) .
	3. Evaluate shading (%) $\text{A}_i=\frac{\text{Y}_i}{\text{Y}_C}\times 100$, where $1\leq i\leq4$.
- Qualified condition
	- $\text{CenterLow}\leq\text{Y}_C\leq\text{CenterUp}$ .
	- $\text{PassLevel}\leq \text{A}_i \leq \text{PassLevelUp}$, where $1\leq i\leq4$ .
	- $\max_{1\leq i\leq4}{A_i}-\min_{1\leq i\leq4}{\text{A}_i}\leq \text{Diff}$.

#### 2. Flare
- Definition: Unwanted light in an image that did not originate from the related scene object
- Reason: 
	-   Everything between the object and the camera's front lens, e.g., haze or dust in the air.
	-   Interreflections between the optical parts of the lens and dust and dirt on the lens elements.
	-   Light reflected from the edges and framing of the lens or the shutter, diaphragm, camera body, etc.
	-   Light from the optical system itself with spherical aberration and chromatic aberrations etc.
	-   Light leakage
- Detect environment: ANSI Contrast
- Detect method
	1. Statistics all pixel value distribution.
	2. Evaluate standard deviation $\sigma$ from pixel distribution.
- Qualified condition
	- $\text{Std_Threshold}\leq\sigma$ .

#### 3. Blemish
- Definition: Visible spots or marks in the image.
- Reason:
	- Sensor defects.
- Detect environment: White card.
- Detect method
	- Pass image to [fast difference filtering](https://www.spiedigitallibrary.org/journals/journal-of-electronic-imaging/volume-18/issue-02/020501/Blemish-detection-in-camera-production-testing-using-fast-difference-filtering/10.1117/1.3132004.full)
	- Pass image to filter to de-noise.
	- Binarize image by $\text{IntensityTolerance}$.
- Qualified condition
	- All pixel intensity $\text{Img}_{ij}<\text{IntensityTolerance}$.
## Example
### Download
```
git clone https://github.com/ed28311/DITCamera.git --recursive [Target Directory]
```

### Build 
```
cd [Target Directory]
mkdir build 
cmake ..
make 
```

### Execute
- Execute singal mission and output report.
```
./bin/main -{mode}{configMode} {config path} {image path}
```
- Execute multiple mission and integrate to one report
```
./bin/main \ 
	 -{mode}{configMode} {config path} {image path} \
	 -{mode}{configMode} {config path} {image path} \
	 ... \
	 -{mode}{configMode} {config path} {image path}
```
## Structure
```
.
├── CameraSpec.spe               -> Basic config file.
├── CMakeLists.txt
├── launch.bat
├── launch.sh
├── README.md
├── LICENSE
├── str
│   ├── algorithmDispatcher.hpp  -> Assign task to correspond algorithm.
│   ├── configLoader.hpp         -> Load Config and constructure config class.
│   ├── DITAlgorithm.hpp         -> Basic algorithm class structure.
│   ├── DITAlgorithms.hpp        -> Header integrate all algorithm.
│   ├── DITConfig.hpp            -> Basic config class structure.
│   ├── logger.hpp               -> To log the result and save as file.
│   ├── method                   -> Directory to storage algorithm.
│   │   ├── blemish.hpp
│   │   ├── flare.hpp
│   │   └── shading.hpp
│   └── tool
│       └── displayer.hpp        -> To prettify string and print variable info.
└── test_pic                     -> Test picture.
    ├── blemish.jpg
    ├── blemishNG.jpg
    ├── blemishOK.jpg
    ├── flare.jpg
    ├── flareNG.jpg
    ├── flareOK.jpg
    ├── shading.jpg
    ├── shadingNG.jpg
    └── shadingOK.jpg

```

#### Class diagram
![ClassDiagram](DITCamera.png)
