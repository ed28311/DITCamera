
SET BASEDIR=%HOMEPATH%\DITCamera
cd %BASEDIR%
start /D "%BASEDIR%\build\bin\Release\" main ^
-BL[Blemish] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\blemishNG.jpg ^
-BL[Blemish] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\blemishOK.jpg ^
-BL[Blemish] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\blemish.jpg ^
-FL[Flare] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\flareNG.jpg ^
-FL[Flare] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\flareOK.jpg ^
-FL[Flare] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\flare.jpg ^
-LS[Shading] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\shadingNG.jpg ^
-LS[Shading] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\shadingOK.jpg ^
-LS[Shading] %BASEDIR%\CameraSpec.spe %BASEDIR%\test_pic\shading.jpg