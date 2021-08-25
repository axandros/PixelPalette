# Write to File
The write to file function is currently unweildy, so I'm going to document it here.

First, we make a vector of unsigned char called fileBytes.  
These are the individual bytes we will be writing to the image file at the end of this function.

## Add Pixel Array
Add pixel array does several things, and should probably be broken up.
The current goals are:
- Calculate all of the values we will need to write to the file.
  - I think the logic here was to hold off on calculating these until we knew we needed them, and the start of this is the earliest we could know.
    Except we could have these in a seperate function we call before setPixelArray.
- Calculate the hex values for the pixels and write them into the fileBytes array.
  - This should be the only purpose of the function.

At the top of addPixel array, I have a note reminding myself that the bytes need to be written in BGR order.

We then call setColorArraySizeBytes

### Set Color Array Size Bytes

First thing we do here is call two other functions. Oi vey.

#### Set Image Width
Finally, a leaf function!
This is where the problems start coming up, because width ends up set to zero.
We get the square root of the hexPixels vecotor's size in order to figure out how to split it into a square.
hexPixel has no contents at this point, however.
This is set in setColorArraySizeBytes, after we get the width, which is used to pad the vector!


#### Set Image Padding