# Counting german license plates and characters on pictures using OpenCV

The data set should contain pictures that vary in quality, weather, light conditions, etc..

1. Image Preprocessing
  - Opening
  - Greyscaling
  - Bluring
  - Thresholding

2. Extract Contrours

3. Iterate over each contour based on aspect ratio, min height and width + remove any unnessecary contours

### Visualized Pipeline: Original Picture and Preprocessing 

<img width="451" alt="image" src="https://user-images.githubusercontent.com/64742664/187214011-3d86742b-b920-4401-a6b9-b83411b33ca9.png">

### Extracted Contours and Result

<img width="415" alt="image" src="https://user-images.githubusercontent.com/64742664/187214257-f2a1163a-8faa-479e-b002-0f1d26a32533.png">

### Current Problems

- Nighttime images
- Pictures with an angle that is too large. The license plate is no longer recognised as a rectangle but a paralelogram.
- License Plates that are to far away

### Copyright Image
Image No: [A201895](https://www.audi-mediacenter.com/en/press-releases/success-model-40-the-new-audi-a3-sportback-12613)
Copyright: AUDI AG
Rights: Use for editorial purposes free of charge
