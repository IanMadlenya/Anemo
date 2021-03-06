#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

/**
Anemo - Digital Image Processing Software for Segmentation of Climatological Graphic Records 

Anemo is a open source software for digital image processing of climatological records registred by mechanical meteorological instruments(like anemometers and barometer).
Curently it's just a prototype that realize the segmentation of anemograms(wind speed only).

Input: Wind speed graphic record(PNG/JPEG/TIFF file).
Output: Segmented graphic with the inflection points detected(PNG/JPEG/TIFF file).

The set of techiques used are Cluster Analysis by K-Means algorithm, Smoothing Algorithm, 
Freeman encoding(Chain code), Chain Code smoothing, Chain Code Profile Analysis and Concavity Analysis.

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

	http://matheussantana.github.com/Anemo/ [Project website]
	https://github.com/matheussantana/Anemo [Code repository]
	http://www2.comp.ufscar.br/~matheus_santana/index.php [More info]
 */


void kmeans(char* argv) {
    /*
     * Cluster analysis - K-Means
    Each class or clusters of objects in image receive a aproximated value of color in
    RGB standart [2], based on the euclidian distance, used for the analysis of the
    similarity between the pixels [10,9].
     *
     * Image segmentation by K-Means algorithm.
    – Step 1: Insert the image and boot: the number of k groups (clusters),
      the initial values of k classes and set the stop criterion;
    – Step 2: Compute the similarity between the pixels of each class;
    – Step 3: Assign each pixel to the group of most similar class;
    – Step 4: Calculate the average of each group and assign it to each
      class according to the groups;
    – Step 5: Check the stopping criterion, if not satisfied, return to step
      2.

     */

    IplImage* img_color = cvLoadImage(argv);
    IplImage* nova = cvCreateImage(cvGetSize(img_color), IPL_DEPTH_8U, 3);

    float v1[3];
    float v2[3];
    float v3[3];
    float vt1[3];
    float vt2[3];
    float vt3[3];
    float alf = 0.6;
    srand(time(NULL));
    CvScalar vc;





    int r, z;
    int w = img_color->width, h = img_color->height;

    v1[0] = 167; //212;//120;//255;//173;//Grade
    v1[1] = 201; //127;//193;//255;//201;
    v1[2] = 128; //156;//212;//197;//141;

    v2[0] = 255; //Fundo
    v2[1] = 255;
    v2[2] = 255;

    v3[0] = 212; //211;//255;//174;//220;//174;//Grafico
    v3[1] = 127; //108;//139;//59;//218;//59;
    v3[2] = 156; //85;//255;//118;//253;//118; Se eu pegar estes valores, os pontos mais claros do grafico sao perdidos. Em contra-partida, nao existe mta distincao grafico-fundo.
    printf("Clusters of objects\n\n:");
    for (int p = 0; p <= 2; p++)
        printf("v1 %f v2 %f v3 %f \n", v1[p], v2[p], v3[p]);


    int i, j, t;
    float d1, d2, d3;
    CvScalar s, m;
    srand(time(NULL));


    do {


        for (i = 0; i <= h - 1; i++) {
            for (j = 0; j <= w - 1; j++) {

                s = cvGet2D(img_color, i, j);
                d1 = sqrt((s.val[0] - v1[0])*(s.val[0] - v1[0]) + (s.val[1] - v1[1])*(s.val[1] - v1[1]) + (s.val[2] - v1[2])*(s.val[2] - v1[2]));
                d2 = sqrt((s.val[0] - v2[0])*(s.val[0] - v2[0]) + (s.val[1] - v2[1])*(s.val[1] - v2[1]) + (s.val[2] - v2[2])*(s.val[2] - v2[2]));
                d3 = sqrt((s.val[0] - v3[0])*(s.val[0] - v3[0]) + (s.val[1] - v3[1])*(s.val[1] - v3[1]) + (s.val[2] - v3[2])*(s.val[2] - v3[2]));
                if (d1 <= d2 && d1 < d3) {
                    s.val[0] = 255;
                    s.val[1] = 0;
                    s.val[2] = 0;
                    cvSet2D(nova, i, j, s);

                }
                else {

                    if (d2 <= d1 && d2 < d3) {
                        s.val[0] = 0;
                        s.val[1] = 255;
                        s.val[2] = 0;
                        cvSet2D(nova, i, j, s);
                    }
                    else {
                        if (d3 <= d1 && d3 < d2) {
                            s.val[0] = 0;
                            s.val[1] = 0;
                            s.val[2] = 255;

                            cvSet2D(nova, i, j, s);
                        } else {
                            printf("d1 %f d2 %f d3 %f \n", d1, d2, d3);
                            for (int p = 0; p <= 2; p++)
                                printf("s %f %d \n", s.val[p], p);
                        }

                    }

                }






            }
        }
        int c1 = 0, c2 = 0, c3 = 0;
        vt1[0] = v1[0];
        vt1[1] = v1[1];
        vt1[2] = v1[2];

        vt2[0] = v2[0];
        vt2[1] = v2[1];
        vt2[2] = v2[2];

        vt3[0] = v3[0];
        vt3[1] = v3[1];
        vt3[2] = v3[2];

        for (i = 0; i <= h - 1; i++) {
            for (j = 0; j <= w - 1; j++) {

                s = cvGet2D(nova, i, j);
                if (s.val[0] == 255 && s.val[1] == 0 && s.val[2] == 0) {
                    c1 = c1 + 1;
                    m = cvGet2D(img_color, i, j);
                    v1[0] = (v1[0] + m.val[0]) / c1;
                    v1[1] = (v1[1] + m.val[1]) / c1;
                    v1[2] = (v1[2] + m.val[2]) / c1;
                } else {

                    if (s.val[1] == 0 && s.val[1] == 255 && s.val[2] == 0) {
                        c2 = c2 + 1;
                        m = cvGet2D(img_color, i, j);
                        v2[0] = (v2[0] + m.val[0]) / c2;
                        v2[1] = (v2[1] + m.val[1]) / c2;
                        v2[2] = (v2[2] + m.val[2]) / c2;
                    }
                    else {

                        if (s.val[1] == 0 && s.val[1] == 0 && s.val[2] == 255) {
                            c3 = c3 + 1;
                            m = cvGet2D(img_color, i, j);
                            v3[0] = (v3[0] + m.val[0]) / c3;
                            v3[1] = (v3[1] + m.val[1]) / c3;
                            v3[2] = (v3[2] + m.val[2]) / c3;
                        }
                    }
                }


            }


        }

    } while (v1[0] != vt1[0] && v2[0] != vt2[0] && v3[0] != vt3[0] && v1[1] != vt1[1] && v2[1] != vt2[1] && v3[1] != vt3[1] && v1[2] != vt1[2] && v2[2] != vt2[2] && v3[2] != vt3[2]);


    cvSaveImage("2.tif", nova);





}

//Neighborhood algorithm.

void construtor(IplImage* img_color) {
    CvScalar s, k; //usado para receber o valor da intensidade de cinza.
    int w = img_color->width, h = img_color->height; // Pega tamanho(linhas e colunas) da matriz de pixels.
    int i, j;
    k.val[0] = 0;
    k.val[1] = 255;
    k.val[2] = 0;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            s = cvGet2D(img_color, i, j);
            if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0) {

                cvSet2D(img_color, i, j, k);
            }
        }
    }
}

int neighborhoodBack6(IplImage* img, int i, int j) {

    int conta = 0;
    CvScalar s;
    s = cvGet2D(img, i - 1, j - 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i, j - 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j - 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i - 1, j + 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i, j + 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j + 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    return conta;

}

int vizgraf6(IplImage* img, int i, int j) {

    int conta = 0;
    CvScalar s;
    s = cvGet2D(img, i - 1, j - 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i, j - 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j - 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i - 1, j + 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i, j + 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j + 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    return conta;

}

int vizfundo8(IplImage* img, int i, int j) {

    int conta = 0;
    CvScalar s;
    s = cvGet2D(img, i - 1, j - 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i, j - 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j - 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i - 1, j + 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i, j + 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j + 1);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;



    s = cvGet2D(img, i - 1, j);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j);

    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0)
        conta = conta + 1;

    return conta;

}

int neighborhoodBack8(IplImage* img, int i, int j) {

    int conta = 0;
    CvScalar s;
    s = cvGet2D(img, i - 1, j - 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i, j - 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j - 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i - 1, j + 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i, j + 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j + 1);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;



    s = cvGet2D(img, i - 1, j);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    s = cvGet2D(img, i + 1, j);

    if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255)
        conta = conta + 1;

    return conta;

}

/**
 Main - Cromaticidade
 */
void neighborhood() {
    /*
     *
     * Removal of neighborhood
The K-means algorithm produces a segmentation appropriated for most of the
pixels in the image, it classifies each pixel in its respective class. However some
classes represent unwanted information, which can be removed by the the anal-
ysis of each pixel and the values of his neighboring pixels. If the majority is
classified as background, using window 6, the pixel in question is upgraded to
background; if its classified as an object, the pixel is updated as the object. In
the case of equality, it is checked using window 8, and if this also fails, the pixel
is updated as a background. Fig.3 illustrates the possible neighbors to a pixel
p. The neighboring pixels marked in red indicate region of object and pixels
marked in green as the background region.

     *
     */

    CvScalar s, m;

    IplImage* nova = cvLoadImage("2.tif");

    IplImage* nova2 = cvCreateImage(cvGetSize(nova), IPL_DEPTH_8U, 3);

    construtor(nova2);



    int w = nova->width, h = nova->height;

    for (int i = 2; i <= h - 2; i++) {
        for (int j = 2; j <= w - 2; j++) {

            s = cvGet2D(nova, i, j);

            if (s.val[0] == 255 && s.val[1] == 0 && s.val[2] == 0) {

                if (neighborhoodBack6(nova, i, j) > vizgraf6(nova, i, j)) {//caso for maior que (240,240,240), faca
                    m.val[0] = 0;
                    m.val[1] = 255;
                    m.val[2] = 0;
                    cvSet2D(nova2, i, j, m);
                } else {
                    if (neighborhoodBack6(nova, i, j) < vizgraf6(nova, i, j)) {//caso for maior que (240,240,240), faca
                        m.val[0] = 0;
                        m.val[1] = 0;
                        m.val[2] = 255;
                        cvSet2D(nova2, i, j, m);
                    } else {
                        if (vizfundo8(nova, i, j) > neighborhoodBack8(nova, i, j)) {//caso for maior que (240,240,240), faca
                            m.val[0] = 0;
                            m.val[1] = 255;
                            m.val[2] = 0;
                            cvSet2D(nova2, i, j, m);
                        } else {
                            if (vizfundo8(nova, i, j) < neighborhoodBack8(nova, i, j)) {//caso for maior que (240,240,240), faca
                                m.val[0] = 0;
                                m.val[1] = 0;
                                m.val[2] = 255;
                                cvSet2D(nova2, i, j, m);
                            } else {
                                m.val[0] = 0;
                                m.val[1] = 255;
                                m.val[2] = 0;
                                cvSet2D(nova2, i, j, m);
                            }

                        }
                    }

                }

            } else {
                if (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) {
                    m.val[0] = 0;
                    m.val[1] = 0;
                    m.val[2] = 255;
                    cvSet2D(nova2, i, j, m);
                } else {
                    if (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0) {
                        m.val[0] = 0;
                        m.val[1] = 255;
                        m.val[2] = 0;
                        cvSet2D(nova2, i, j, m);
                    }
                }
            }

        }


    }


    cvSaveImage("3.tif", nova2);



}

//Filtro de Media

void AverageFilterSmooth() {
    const char* name = "Filters Window";
    IplImage* img = cvLoadImage("3.tif");
    IplImage* out = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);

    cvSmooth(img, out, CV_MEDIAN, 5, 3);

    cvSaveImage("4.tif", out);


}

//Chain Code

int u, v, dir = 3;
bool sozinho = false;

void dir_next(int i, int j, IplImage* img_color) {
    CvScalar s; //usado para receber o valor da intensidade de cinza.

    if (dir == 3) {
        s = cvGet2D(img_color, i, j + 1);
        if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
            u = i;
            v = j + 1;
            dir = 2;
        } else {
            s = cvGet2D(img_color, i + 1, j);
            if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                u = i + 1;
                v = j;
                dir = 3;
            } else {
                s = cvGet2D(img_color, i, j - 1);
                if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                    u = i;
                    v = j - 1;
                    dir = 4;
                } else {
                    s = cvGet2D(img_color, i - 1, j);
                    if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                        u = i - 1;
                        v = j;
                        dir = 1;
                    } else {
                        u = i;
                        v = j;
                        dir = -3;
                    }

                }
            }
        }
    } else {
        if (dir == 2) {
            s = cvGet2D(img_color, i - 1, j);
            if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                u = i - 1;
                v = j;
                dir = 1;
            } else {
                s = cvGet2D(img_color, i, j + 1);
                if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                    u = i;
                    v = j + 1;
                    dir = 2;
                } else {
                    s = cvGet2D(img_color, i + 1, j);
                    if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                        u = i + 1;
                        v = j;
                        dir = 3;
                    } else {
                        s = cvGet2D(img_color, i, j - 1);
                        if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                            u = i;
                            v = j - 1;
                            dir = 4;
                        } else {
                            u = i;
                            v = j;
                            dir = -3;
                        }

                    }
                }
            }
        } else {
            if (dir == 1) {
                s = cvGet2D(img_color, i, j - 1);
                if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                    u = i;
                    v = j - 1;
                    dir = 4;
                } else {
                    s = cvGet2D(img_color, i - 1, j);
                    if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                        u = i - 1;
                        v = j;
                        dir = 1;
                    } else {
                        s = cvGet2D(img_color, i, j + 1);
                        if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                            u = i;
                            v = j + 1;
                            dir = 2;
                        } else {
                            s = cvGet2D(img_color, i + 1, j);
                            if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                                u = i + 1;
                                v = j;
                                dir = 3;
                            } else {
                                u = i;
                                v = j;
                                dir = -3;
                            }

                        }
                    }
                }
            } else {
                if (dir == 4) {
                    s = cvGet2D(img_color, i + 1, j);
                    if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                        u = i + 1;
                        v = j;
                        dir = 3;
                    } else {
                        s = cvGet2D(img_color, i, j - 1);
                        if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                            u = i;
                            v = j - 1;
                            dir = 4;
                        } else {
                            s = cvGet2D(img_color, i - 1, j);
                            if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                                u = i - 1;
                                v = j;
                                dir = 1;
                            } else {
                                s = cvGet2D(img_color, i, j + 1);
                                if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) || (s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0)) {
                                    u = i;
                                    v = j + 1;
                                    dir = 2;
                                } else {
                                    u = i;
                                    v = j;
                                    dir = -3;
                                }

                            }
                        }
                    }
                }
            }
        }
    }
}

void BuscaPixelSozinho(int h, int w, IplImage* img_color) {

    CvScalar s, t, r, z, k;
    CvScalar x;

    x.val[0] = 0;
    x.val[1] = 255;
    x.val[2] = 0;



    for (int i = 1; i < h - 1; i++) {
        for (int j = 1; j < w - 1; j++) {

            k = cvGet2D(img_color, i, j);
            s = cvGet2D(img_color, i - 1, j - 1);
            t = cvGet2D(img_color, i - 1, j + 1);
            r = cvGet2D(img_color, i + 1, j + 1);
            z = cvGet2D(img_color, i + 1, j - 1);


            if ((k.val[0] == 0 && k.val[1] == 0 && k.val[2] == 255) && (s.val[0] == 0 && s.val[1] == 255 && s.val[2] == 0) && (t.val[0] == 0 && t.val[1] == 255 && t.val[2] == 0) && (r.val[0] == 0 && r.val[1] == 255 && r.val[2] == 0) && (z.val[0] == 0 && z.val[1] == 255 && z.val[2] == 0)) {
                //cout << "i: " << i << "j: " << j << endl;
                cvSet2D(img_color, i, j, x);
            }
        }
    }
}

void ChainCode(char *file) {
    /*
     * Freeman encoding
     * 
     * The Chain Code algorithm by Freeman [1,3] is used to describe, using a string,
the behavior of the contour of objects. This algorithm uses for orientation four
possible directions (4-segments): North (2), South (6), Eastern (0) and West (4),
or eight directions: North-East (1), North-West (3), South-East (7) and South-
West (5).

     *
     * */
    IplImage* img_color = cvLoadImage("4.tif");



    bool busca = 1;
    int w = img_color->width, h = img_color->height; // Pega tamanho(linhas e colunas) da matriz de pixels.
    CvScalar s, t1, k; //usado para receber o valor da intensidade de cinza.
    ofstream fgrava(file);


    k.val[0] = 0;
    k.val[1] = 0;
    k.val[2] = 0;

    int i, j, t, p, conta = 0;

    for (i = 1; i < h - 1; i++) {
        for (j = 1; j < w - 1; j++) {
            s = cvGet2D(img_color, i, j);
            t1 = cvGet2D(img_color, i, j + 1);
            if (busca == 1) {


                if (((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) && (t1.val[0] == 0 && t1.val[1] == 0 && t1.val[2] == 255)) || ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) && (t1.val[0] == 0 && t1.val[1] == 255 && t1.val[2] == 0))) {//red-> redd

                    fgrava.write((char *) & i, sizeof (i));


                    fgrava.write((char *) & j, sizeof (j));

                    conta++;
                    t = i;
                    p = j;
                    do {

                        cvSet2D(img_color, i, j, k);
                        dir_next(i, j, img_color);


                        i = u;
                        j = v;

                        if (dir == -3) {
                            fgrava.write((char *) & dir, sizeof (dir));
                            fgrava.write((char *) & dir, sizeof (dir));
                            dir = 3;
                        } else {
                            fgrava.write((char *) & dir, sizeof (dir));
                        }


                    } while (i != t || j != p);
                    int var = -1;

                    fgrava.write((char *) & var, sizeof (var));

                    fgrava.write((char *) & var, sizeof (var));

                } else {

                    if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0) && (t1.val[0] == 0 && t1.val[1] == 0 && t1.val[2] == 255))
                        busca = 0;
                    else {

                        if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0) && (t1.val[0] == 0 && t1.val[1] == 255 && t1.val[2] == 0))
                            busca = 1;
                        else {
                            if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0) && (t1.val[0] == 0 && t1.val[1] == 0 && t1.val[2] == 0))
                                busca = 0;
                        }
                    }
                }
            } else {

                if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 0) && (t1.val[0] == 0 && t1.val[1] == 255 && t1.val[2] == 0))
                    busca = 1;
                if ((s.val[0] == 0 && s.val[1] == 0 && s.val[2] == 255) && (t1.val[0] == 0 && t1.val[1] == 255 && t1.val[2] == 0))
                    busca = 1;
            }

        }
        busca = 1;
    }
    int var2 = -2;


    fgrava.write((char *) & var2, sizeof (var2));

    fgrava.write((char *) & var2, sizeof (var2));

    fgrava.close();
    cvSaveImage("5.tif", img_color);

    cvReleaseImage(&img_color);
}


//Smothing ChainCode

bool buscaD(int v[2]) {

    if ((v[0] == 4 && v[1] == 2) || (v[0] == 2 && v[1] == 4) || (v[0] == 1 && v[1] == 3) || (v[0] == 3 && v[1] == 1))
        return 1;
    else
        return 0;
}

bool buscaE(int v[2]) {
    if ((v[0] == 7 && v[1] == 5) || (v[0] == 5 && v[1] == 7) || (v[0] == 6 && v[1] == 8) || (v[0] == 8 && v[1] == 6))
        return 1;
    else
        return 0;
}

int buscaB(int v[2]) {
    if ((v[0] == 6 && v[1] == 5) || (v[0] == 5 && v[1] == 6))
        return 2;
    else {
        if ((v[0] == 6 && v[1] == 7) || (v[0] == 7 && v[1] == 6))
            return 3;
        else {
            if ((v[0] == 7 && v[1] == 8) || (v[0] == 8 && v[1] == 7))
                return 4;
            else {
                if ((v[0] == 5 && v[1] == 8) || (v[0] == 8 && v[1] == 5))
                    return 1;
                else
                    return 9;

            }
        }
    }
}

int buscaC(int v[2]) {

    if ((v[0] == 3 && v[1] == 5) || (v[0] == 5 && v[1] == 3) || (v[0] == 1 && v[1] == 6) || (v[0] == 6 && v[1] == 1))
        return 2;
    else {
        if ((v[0] == 2 && v[1] == 7) || (v[0] == 4 && v[1] == 6) || (v[0] == 7 && v[1] == 2) || (v[0] == 6 && v[1] == 4))
            return 3;
        else
            return 9;
    }
}

int buscaA(int v[2]) {
    if ((v[0] == 2 && v[1] == 3) || (v[0] == 3 && v[1] == 2))
        return 6;
    else {
        if ((v[0] == 2 && v[1] == 1) || (v[0] == 1 && v[1] == 2))
            return 5;
        else {
            if ((v[0] == 4 && v[1] == 3) || (v[0] == 3 && v[1] == 4))
                return 7;
            else {
                if ((v[0] == 4 && v[1] == 1) || (v[0] == 1 && v[1] == 4))
                    return 8;
                else
                    return 9;
            }
        }
    }
}

void SmothingChainCode(char *file, char *file2) {

    /*
     *
     * Chain Code smoothing
The string produced by Chain Code algorithm generates redundant and unnec-
essary code to describe the contour, this way it is applied a second technique,
which smooths this sequence [7] by analyzing every two characters, replacing,
when necessary, with a character that describes the same behavior by using the
8-directions illustrated in fig.4(b), and is produced as a result a lower sequence.

     *
     */

    int v[2], ba, bb, bc, out[900000], conta = 0, i, t, px, py, ct = 0, ct2 = 0, var;
    bool achou = 0, exec = 0;
    ifstream arq(file);
    ofstream arq2(file2);

    arq.read((char *) & var, sizeof (var));
    v[0] = var;
    arq.read((char *) & var, sizeof (var));
    v[1] = var;


    while (v[0] != -2 && v[1] != -2) {//fim de todos os chain code na imagem.
        if (v[0] != -1 && v[1] != -1) {
            px = v[0]; //px = ponto inicial x.
            py = v[1]; //py = ponto inicial y.
            arq.read((char *) & var, sizeof (var));
            v[0] = var;
            arq.read((char *) & var, sizeof (var));
            v[1] = var;

        }


        while (v[0] != -1 && v[1] != -1) {//fim de um chain code de um objeto na imagem.
            exec = 1;
            if (v[0] == v[1]) {

                out[conta] = v[0];
                conta++;
                achou = 0;
            }
            else {
                if (buscaD(v) == 1 || buscaE(v) == 1) {
                    achou = 1;
                } else {
                    ba = buscaA(v);
                    if (ba != 9) {
                        out[conta] = ba;
                        conta++;
                        achou = 1;
                    } else {
                        bb = buscaB(v);
                        if (bb != 9) {
                            out[conta] = bb;
                            conta = conta + 1;
                            out[conta] = bb;
                            conta++;
                            achou = 1;
                        } else {
                            bc = buscaC(v);
                            if (bc != 9) {
                                out[conta] = bc;
                                conta++;
                                achou = 1;
                            } else
                                out[conta] = v[0];
                            conta = conta + 1;
                            achou = 0;
                        }
                    }
                }
            }
            //    cout << "out:" <<out[conta-1]<< endl;
            //cout << v[0] << v[1] << endl;
            //cout << achou << "-" << v[0] << "-" << v[1] <<  endl;
            if (achou == 1) {
                arq.read((char *) & var, sizeof (var));
                v[0] = var;
                arq.read((char *) & var, sizeof (var));
                v[1] = var;
            } else {

                if (achou == 0) {
                    v[0] = v[1];
                    arq.read((char *) & var, sizeof (var));
                    v[1] = var;
                }
            }
            if (v[0] != -1 && v[1] == -1) {
                out[conta] = v[0];
                v[0] = v[1];
                arq.read((char *) & var, sizeof (var));
                v[1] = var;
                conta++;
            }
            //cout << px << "-" << py << endl;
        }

        //cout << "saída simplificada - " << "P(x,y): " << px << "-" << py << endl;
        //cout << "conta: " << conta << endl;

        if (exec == 1 && conta > 1) {
            arq2.write((char *) & px, sizeof (px));
            arq2.write((char *) & py, sizeof (py));

            //cout << px << endl;
            //cout << py << endl;
            //cout << endl;


            for (i = 0; i <= conta - 1; i++) {
                if (out[i] != 0)
                    arq2.write((char *) & out[i], sizeof (i));
                //cout << out[i] << endl;
            }
            conta = 0;
            arq.read((char *) & var, sizeof (var));
            v[0] = var;
            arq.read((char *) & var, sizeof (var));
            v[1] = var;
            //cout << endl;

            int svar = -1;

            arq2.write((char *) & svar, sizeof (svar));
            arq2.write((char *) & svar, sizeof (svar));
            /*    cout << endl;
                cout << -1 << endl;
                cout << -1 << endl;
                cout << endl;
             */
        } else {
            conta = 0;
            arq.read((char *) & var, sizeof (var));
            v[0] = var;
            arq.read((char *) & var, sizeof (var));
            v[1] = var;
        }
    }

    int svar2 = -2;
    arq2.write((char *) & svar2, sizeof (svar2));
    arq2.write((char *) & svar2, sizeof (svar2));

    //    cout << -2 << endl;
    //    cout << -2 << endl;

    arq.close();
    arq2.close();
    //    cout << conta << endl;

}

//Construtor ChainCode

void ConstructorChainCode(char *file, char* argv) {

    //Create binary image and plot chain code;

    int v[2], px, py, ct = 0, var;
    CvScalar k, s;
    IplImage* img_color = cvLoadImage(argv);
    IplImage* nova = cvCreateImage(cvGetSize(img_color), IPL_DEPTH_8U, 3);
    int w = img_color->width, h = img_color->height;
    ifstream arq(file);

    s.val[0] = 0;
    s.val[1] = 0;
    s.val[2] = 0;

    for (int i = 0; i <= h - 1; i++) {
        for (int j = 0; j <= w - 1; j++) {
            cvSet2D(nova, i, j, s);

        }
    }

    arq.read((char *) & var, sizeof (var));
    v[0] = var;
    arq.read((char *) & var, sizeof (var));
    v[1] = var;


    k.val[0] = 255;
    k.val[1] = 255;
    k.val[2] = 255;

    while (v[0] != -2 || v[1] != -2) {
        //      cout << v[0] << "-" << v[1] << endl;
        if (v[0] != -1) {
            px = v[0];
            py = v[1];
            cvSet2D(nova, px, py, k);
            arq.read((char *) & var, sizeof (var));
            v[0] = var;
        }

        while (v[0] != -1) {
            //      cout << "aa: " << px << "-" << py << endl;
            if (v[0] == 1) {
                px = px - 1;
                py = py;
                cvSet2D(nova, px, py, k);
            } else {
                if (v[0] == 2) {
                    px = px;
                    py = py + 1;
                    cvSet2D(nova, px, py, k);
                } else {
                    if (v[0] == 3) {
                        px = px + 1;
                        py = py;
                        cvSet2D(nova, px, py, k);
                    } else {
                        if (v[0] == 4) {
                            px = px;
                            py = py - 1;
                            cvSet2D(nova, px, py, k);
                        } else {
                            if (v[0] == 5) {
                                px = px - 1;
                                py = py + 1;
                                cvSet2D(nova, px, py, k);
                            } else {
                                if (v[0] == 6) {
                                    px = px + 1;
                                    py = py + 1;
                                    cvSet2D(nova, px, py, k);
                                } else {
                                    if (v[0] == 7) {
                                        px = px + 1;
                                        py = py - 1;
                                        cvSet2D(nova, px, py, k);
                                    } else {
                                        if (v[0] == 8) {
                                            px = px - 1;
                                            py = py - 1;
                                            cvSet2D(nova, px, py, k);
                                        } else {
                                            if (v[0] == -3) {
                                                px = px;
                                                py = py;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //       cout << v[0] << "-" << px << "-" << py << endl;
            arq.read((char *) & var, sizeof (var));
            v[0] = var;
            //    	cvSaveImage("2.tif", nova);
            //arq.read((char *)&var, sizeof(var)); v[1] = var;
            //        cout << v[0] << endl;
        }
        //cout << v[0] << "-" << v[1] << endl;
        arq.read((char *) & var, sizeof (var));
        v[0] = var;
        arq.read((char *) & var, sizeof (var));
        v[1] = var;
        v[0] = v[1];
        arq.read((char *) & var, sizeof (var));
        v[1] = var;
    }
    cvSaveImage("6.tif", nova);
    //    	cvSaveImage("8.tif", img_color);
}

//Superior e Inferior - Medias

void FindUpper(IplImage* img_color, float sup[], int w, int h) {
    CvScalar s; //usado para receber o valor da intensidade de cinza.
    bool achou = false;
    int conta = 0, i, j;
    //cout<<"oi"<< endl;
    for (j = 0; j < w - 1; j++) {
        for (i = 0; i < h - 1; i++) {
            //cout << i << "-"<< j << endl;
            s = cvGet2D(img_color, i, j);
            if (s.val[0] == 255 && achou == 0) {
                achou = 1;
                sup[conta] = i;
                conta++;
            }
        }
        if (achou == 0) {
            sup[conta] = 0;
            conta++;
        }
        achou = false;
    }
    //	cout << "ble" << endl;
}

void FindLower(IplImage* img_color, float inf[], int w, int h) {
    CvScalar s; //usado para receber o valor da intensidade de cinza.
    bool achou = false;
    int conta = 0, i, j;
    //cout<<"oi1"<< endl;

    for (j = 0; j <= w - 1; j++) {
        for (i = h - 1; i >= 0; i--) {
            //cout << i << "-"<< j << endl;

            s = cvGet2D(img_color, i, j);
            if (s.val[0] == 255 && achou == 0) {
                achou = 1;
                inf[conta] = i;
                conta++;
            }
        }
        if (achou == 0) {
            inf[conta] = 0;
            conta++;
        }
        achou = false;
    }
}

void MakeImgUpper(IplImage* img, int tam, float *vt) {
    int j;
    CvScalar k;

    k.val[0] = 255;
    k.val[1] = 255;
    k.val[2] = 255;

    for (j = 0; j <= tam - 1; j++) {
        if (vt[j] != 0) {
            cvSet2D(img, vt[j], j, k);
        }
    }
}

void tMakeImgLower(IplImage* img, int tam, float *vt) {
    int j;
    CvScalar k;

    k.val[0] = 255;
    k.val[1] = 255;
    k.val[2] = 255;

    for (j = 0; j <= tam - 1; j++) {
        if (vt[j] != 0) {
            //cout << vt[j] << "|" << j << endl;
            cvSet2D(img, vt[j], j, k);
        }
    }
}

bool vizpreto(float *vt, int in, int val) {
    bool achou = false;

    for (int i = in; i <= in + val; i++) {
        if (vt[i] == 0)
            achou = true;

    }

    for (int i = in; i >= in - val; i--) {
        if (vt[i] == 0)
            achou = true;
    }

    return achou;
}

void avg(float *vt, int k, int tam, float *vtOut) {
    int i, j, val;
    float aux = 0;
    val = k / 2;
    for (i = val; i <= (tam - (k - val)); i++) {
        if (vt[i] != 0 && vizpreto(vt, i, val) == false) {
            //cout << i << endl;
            for (j = i - (k / 2); j <= i + (k / 2); j++) {
                if (i + (k / 2) <= (tam - (k - val))) {
                    //cout << "j: " << j << endl;
                    aux = aux + vt[j];
                }
            }
            vtOut[i] = aux / k;
            aux = 0;
        } else {
            if (vt[i] == 0)
                vtOut[i] = 0;
            else {
                if (vizpreto(vt, i, val) == true)
                    vtOut[i] = vt[i];
            }
        }
    }
}

void UpperLower(char* argv) {

    /*
     *        Upper and lower profiles.
The characters sequence generated by the Chain Code algorithm represents
the entire behavior of the objects contour, however, its necessary to represent
the graphic behavior only the upper (maximum values) and lower pixel’s pro-
files(minimum values), so,to find the upper profile, for each column of the image,
the first white pixel(With a top-down/left-right scan) are organized into a string
sequence and for each position of the sequence receive the row value of these
white pixel. The process to find the lower profile are similar, the scan are made
also for each column but starting with a bottom-up scan. To reduce noise and
lost data we use the compute of average for each element in the string

     * */

    IplImage* img_color = cvLoadImage("6.tif");
    //    IplImage* novaSupInf = cvLoadImage( "2d.tif");
    IplImage* img = cvLoadImage(argv);

    int w = img_color->width, h = img_color->height; // Pega tamanho(linhas e colunas) da matriz de pixels.
    IplImage* novaSup = cvCreateImage(cvGetSize(img_color), IPL_DEPTH_8U, 1);
    IplImage* novaInf = cvCreateImage(cvGetSize(img_color), IPL_DEPTH_8U, 1);
    IplImage* novaSupInf = cvCreateImage(cvGetSize(img_color), IPL_DEPTH_8U, 3);

    //	cout << w << "-" << h << endl;
    int k;
    float sup[w];
    float inf[w];
    float out[w];
    float out2[w];

    CvScalar k1;

    k1.val[0] = 0;
    k1.val[1] = 0;
    k1.val[2] = 0;

    for (int j = 0; j <= w; j++) {
        sup[j] = 0;
        inf[j] = 0;
        out[j] = 0;
        out2[j] = 0;
    }

    FindLower(img_color, inf, w, h);
    FindUpper(img_color, sup, w, h);
    /*    int i;
        for(i=0;i<=w-1;i++)
        cout << sup[i] << endl;*/
    cout << endl;
    cout << "Average size window: " << endl;
    cin >> k;
    if (k != 0) {
        avg(inf, k, w, out);
        avg(sup, k, w, out2);

        //    MontaImgInf(novaSup, w, out);

        //    MontaImgSup(novaInf, w, out2);

        tMakeImgLower(novaSupInf, w, out);
        MakeImgUpper(novaSupInf, w, out2);

        tMakeImgLower(img, w, out);
        MakeImgUpper(img, w, out2);

        //    cvSaveImage("Sup.tif", novaSup);
        //    cvSaveImage("Inf.tif", novaInf);
        cvSaveImage("7.tif", novaSupInf);
    }
    else {
        tMakeImgLower(novaSupInf, w, sup);
        MakeImgUpper(novaSupInf, w, inf);

        tMakeImgLower(img, w, sup);
        MakeImgUpper(img, w, inf);

        //    cvSaveImage("Sup.tif", novaSup);
        //    cvSaveImage("Inf.tif", novaInf);
        cvSaveImage("7.tif", novaSupInf);
    }
    cvSaveImage("8.tif", img);


}

//Pontos de Maximos e Minimos

int in = 1;
int inicio = 0;

void MakeImgUpDif2(IplImage* img, int tam, float *vt, float *vtDif1, float *vtDif2) {

    int j;
    CvScalar k;

    k.val[0] = 255;
    k.val[1] = 255;
    k.val[2] = 255;
    //cout << "inicio: " << inicio << endl;
    for (j = inicio + 1; j <= tam - 1; j++) {

        if ((vtDif1[j] == -2 || vtDif1[j] == 2) && (vtDif2[j] > 0 || vtDif2[j] < 0)) {
            /*cout << vtDif1[j] << "|" << vtDif2[j] << endl;*/
            //            if((vtDif[j+1]>0.0 && vtDif[j]<0.0)||(vtDif[j+1]<0.0 && vtDif[j]>0.0)){//+ --> - e - --> +
            //		cout << vtDif[j] << " | "  << vtDif[j+1] << endl;

            cvSet2D(img, vt[j], j, k);
        }
    }
}

void MakeImgLow(IplImage* img, int tam, float *vt) {
    int j;
    CvScalar k;

    k.val[0] = 255;
    k.val[1] = 0;
    k.val[2] = 0;

    for (j = 0; j <= tam - 1; j++) {
        if (vt[j] != 0) {
            cvSet2D(img, vt[j], j, k);
        }
    }
}

void diferencial(float *vt, int tam, float *vtOut) {
    int i;
    float dif = 0;
    bool achou = false;

    vtOut[0] = 0;
    vtOut[tam - 1] = vt[tam - 1];

    for (i = in; i <= tam; i++) {
        if (vt[i] != 0 && achou == false) {
            inicio = i;
            achou = true;
        }
        dif = vt[i] - vt[i - 1];
        vtOut[i] = dif;
        //            cout << dif << endl;
        dif = 0;
    }
    in++;
    inicio++;
}

void vectorSup(float *vt, int tam, IplImage* img) {
    int i;
    float dif = 0;
    bool achou = false;
    CvScalar k, s, t, z, r;

    k.val[0] = 0; //Preto
    k.val[1] = 0;
    k.val[2] = 0;

    t.val[0] = 0; //Vermelho
    t.val[1] = 0;
    t.val[2] = 255;
    /*
        k.val[0] = 0;//Preto
        k.val[1] = 0;
        k.val[2] = 0;

        t.val[0] = 0;//Vermelho
        t.val[1] = 0;
        t.val[2] = 0;*/

    int maior = 0, menor = 0, posMaior = 0, posMenor = 0, j = 0;

    for (i = 1; i <= tam - 1; i++) {
        if (vt[i] != 0) {
            if ((((vt[i] - vt[i - 4]) > 0) && (vt[i + 4] - vt[i]) < 0) || ((vt[i] - vt[i - 4]) < 0) && (vt[i + 4] - vt[i]) > 0) {
                if (((vt[i] - vt[i - 4]) > 0) && (vt[i + 4] - vt[i]) < 0) {
                    // cout << (vt[i]-vt[i-4]) << "a|" << (vt[i+4]-vt[i]) << endl;
                    //            maior = 0;
                    maior = vt[i];

                    j = i;
                    do {
                        if (vt[j] >= maior) {
                            maior = vt[j];
                            posMaior = j;
                        }
                        j++;
                    } while (((vt[j] - vt[j - 4]) > 0) && ((vt[j + 4] - vt[j]) < 0) && vt[j] != 0);
                    cvSet2D(img, maior, posMaior, k);
                } else {
                    if (((vt[i] - vt[i - 4]) < 0) && (vt[i + 4] - vt[i]) > 0) {
                        //          cout << (vt[i]-vt[i-4]) << "b|" << (vt[i+4]-vt[i]) << endl;
                        //cout << i << endl;
                        j = i;
                        menor = vt[i];
                        //-+
                        do {

                            if (vt[j] <= menor) {
                                menor = vt[j];
                                posMenor = j;
                            }
                            j++;
                        } while (((vt[j] - vt[j - 4]) < 0) && ((vt[j + 4] - vt[j]) > 0) && vt[j] != 0);
                        cvSet2D(img, menor, posMenor, t);
                    }
                }
                i = j;


                posMaior = 0;
                posMenor = 0;
            }
        }
    }

    //}
}

void vectorInf(float *vt, int tam, IplImage* img) {
    int i;
    float dif = 0;
    bool achou = false;
    CvScalar k, s, t, z, r;
    int maior = 0, menor = 0, posMaior = 0, posMenor = 0, j = 0;

    k.val[0] = 0; //Verde
    k.val[1] = 255;
    k.val[2] = 0;

    t.val[0] = 0; //Amarelo
    t.val[1] = 255;
    t.val[2] = 255;
    /*
        k.val[0] = 0;//Verde
        k.val[1] = 0;
        k.val[2] = 0;

        t.val[0] = 0;//Amarelo
        t.val[1] = 0;
        t.val[2] = 0;*/




    for (i = 1; i <= tam - 1; i++) {
        if (vt[i] != 0) {
            if ((((vt[i] - vt[i - 4]) > 0) && (vt[i + 4] - vt[i]) < 0) || ((vt[i] - vt[i - 4]) < 0) && (vt[i + 4] - vt[i]) > 0) {
                if (((vt[i] - vt[i - 4]) > 0) && (vt[i + 4] - vt[i]) < 0) {
                    // cout << (vt[i]-vt[i-4]) << "a|" << (vt[i+4]-vt[i]) << endl;
                    //            maior = 0;
                    maior = vt[i];

                    j = i;
                    do {
                        if (vt[j] >= maior) {
                            maior = vt[j];
                            posMaior = j;
                        }
                        j++;
                    } while (((vt[j] - vt[j - 4]) > 0) && ((vt[j + 4] - vt[j]) < 0) && vt[j] != 0);
                    cvSet2D(img, maior, posMaior, k);
                } else {
                    if (((vt[i] - vt[i - 4]) < 0) && (vt[i + 4] - vt[i]) > 0) {
                        //cout << (vt[i]-vt[i-4]) << "b|" << (vt[i+4]-vt[i]) << endl;
                        //cout << i << endl;
                        j = i;
                        menor = vt[i];
                        //-+
                        do {

                            if (vt[j] <= menor) {
                                menor = vt[j];
                                posMenor = j;
                            }
                            j++;
                        } while (((vt[j] - vt[j - 4]) < 0) && ((vt[j + 4] - vt[j]) > 0) && vt[j] != 0);
                        cvSet2D(img, menor, posMenor, t);
                    }
                }
                i = j;


                posMaior = 0;
                posMenor = 0;
            }
        }
    }

    //}
}

void display(float *vt, int tam) {

    for (int i = 0; i <= tam - 1; i++) {
        cout << vt[i] << endl;
    }

}

void InflectionPoint(char *argv, char *out) {
    /*
     * Points of maximum and minimum.
To detect the maximum and minimum points, we analyzed the concavity of the
graphic by checking the inflection values for the upper and lower profiles.
     * Algorithm
     *   Initialize:
– Set T as upper or lower Chain Code sequence;
– Set i = 0; Set {α | α is the vector magnitude, α ∈ N − {0}};
  Iteration: i+α...[size(T)-α];
– If (((T[i ]-T[i -α] > 0) AND (T[i +α]-T[i ] < 0)) OR ((T[i ]-T[i -α] <
  0) AND (T[i +α]-T[i ] > 0)));
      If (T[i ] is the max/min value in the [i+α ... i-α] array); Set
                 ̇
      T[i ] = X;

     */

    IplImage* img_color = cvLoadImage("7.tif");
    IplImage *img = cvLoadImage(argv);
    IplImage* img2 = cvLoadImage("8.tif");

    int w = img_color->width, h = img_color->height; // Pega tamanho(linhas e colunas) da matriz de pixels.
    IplImage* novaSup = cvCreateImage(cvGetSize(img_color), IPL_DEPTH_8U, 3);



    float sup[w];
    float inf[w];
    float dif[w];
    float dif2[w];
    float dif3[w];
    float dif4[w];

    FindLower(img_color, inf, w, h);
    FindUpper(img_color, sup, w, h);

    vectorSup(sup, w, img2);
    vectorInf(inf, w, img2);

    //display(inf,w);

    /*diferencial(sup, w, dif);//Aplica primeira derivada.
    diferencial(inf, w, dif2);//Aplica primeira derivada.

    diferencial(dif, w, dif3);//Aplica segunda derivada.
    diferencial(dif2, w, dif4);//Aplica segunda derivada.*/

    //display(dif3,w);
    //MontaImgInf(novaSup, w, sup);//plota imagem.
    //MontaImgInf(novaSup, w, inf);//plota imagem.
    //MontaImgSupDif2(novaSup, w, sup, dif, dif3);//Plota derivadas.
    //co0ut << "Inferior:" << endl;
    //MontaImgSupDif2(novaSup, w, inf, dif2, dif4);//Plota derivadas.



    int SaveReturn = cvSaveImage(out, img2);


    if (SaveReturn == 1)

        cout << "Sucess! Output saved as " << out << endl;



}

int main(int argc, char** argv) {

    printf("Anemo\n");
    printf("Digital Image Processing Software for Segmentation of Climatological Graphic Records\n");
    printf("Federal University of São Carlos(UFSCar), Brazil\n");


    printf("Version: 0.1 http://www.imagemagick.org\n\
Copyright (C) 2012 Matheus Santana\n");
    printf("mode: Anemogram(wind speed)\n\n");
    char chaincode[50];
    char chaincode2[50];
    char chaincode3[50];


    if (argc < 2) {

        printf("Usage: %s <image input> <image output - optional>\n", argv[0]);


    } else {


        kmeans(argv[1]);
        neighborhood();
        AverageFilterSmooth();
        strcpy(chaincode, "chaincode.txt");
        strcpy(chaincode2, "chaincode2.txt");
        strcpy(chaincode3, "chaincode3.txt");

        ChainCode(chaincode);
        SmothingChainCode(chaincode, chaincode2);

        SmothingChainCode(chaincode2, chaincode3);
        ConstructorChainCode(chaincode3, argv[1]);

        UpperLower(argv[1]);

        if (argc == 3)
            InflectionPoint(argv[1], argv[2]);
        else
            InflectionPoint(argv[1], "output.tiff");
    }
}
