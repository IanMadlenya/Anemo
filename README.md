# Anemo

## About

*Anemo - Digital Image Processing Software for Segmentation of Climatological Graphic Records*

*Anemo* is a open source software for digital image processing of climatological records registred by mechanical meteorological instruments(like anemometers and barometer).
Curently it's just a prototype that realize the segmentation of anemograms(wind speed only).

**Input:** Wind speed graphic record(PNG/JPEG/TIFF file).

**Output:** Segmented graphic with the inflection points detected(PNG/JPEG/TIFF file).

Example:

Input

![alt text](http://www2.comp.ufscar.br/~matheus_santana/images/examples/input.png "Input")

K-Means

![alt text](http://www2.comp.ufscar.br/~matheus_santana/images/examples/2.png "k-means")

Removal of neighbor

![alt text](http://www2.comp.ufscar.br/~matheus_santana/images/examples/3.png "neighbor")

Average filter

![alt text](http://www2.comp.ufscar.br/~matheus_santana/images/examples/4.png "avgfilter")


Chain Code Sequence

![alt text](http://www2.comp.ufscar.br/~matheus_santana/images/examples/6.png "chaincode")

Upper and lower profiles with average 3

![alt text](http://www2.comp.ufscar.br/~matheus_santana/images/examples/7.png "upperlower")

Inflection points

![alt text](http://www2.comp.ufscar.br/~matheus_santana/images/examples/output.png "upperlower")

The set of techiques used are Cluster Analysis by K-Means algorithm, Smoothing Algorithm, 
Freeman Encoding(Chain code), Chain Code Smoothing, Chain Code Profile Analysis and Concavity Analysis.

    Copyright (C) 2012 Matheus Santana
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

Created by: Matheus Santana (matheusslima <at> yahoo <dot> com <dot> br)

Date: 17/08/2012

For futher information see:

[Project website] (http://matheussantana.github.com/Anemo/)

[Code repository] (https://github.com/matheussantana/Anemo)

[More info] (http://www2.comp.ufscar.br/~matheus_santana/static.php?page=static120818-035738)

##Requirements

* OpenCV X or greater.

##Installation

  1. Download and unzip the .zip file or navigate to your git cloned directory
  2. Go to directory bin/
  3. Open a terminal and run:
    chmod +x anemo
    ./anemo <image input> <image output - optional>

You can use for test the input the images inside the inputs/ directory


# To do list

BUGS


Improvements


New config settings:


##Version History

    First Release:
    0.1 - 04/09/2004
    Release the code on GitHub under the GNU Public License


##License

The Anemo is released under the GNU Public License 3.

You are free to use and modify the Anemo. All changes 
must be uploaded to Git Hub under Anemo.
