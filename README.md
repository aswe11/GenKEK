---
stylesheet: https://cdnjs.cloudflare.com/ajax/libs/github-markdown-css/2.10.0/github-markdown.min.css
body_class: markdown-body
pdf_options:
  format: A4
  margin: 24mm 16mm
  displayHeaderFooter: true
  headerTemplate: |-
    <style>
      section {
        margin: 0 auto;
        font-family: system-ui;
        font-size: 11px;
      }
    </style>
    <section>
    </section>
  footerTemplate: |-
    <section>
      <div>
        <span class="pageNumber"></span>
        <!-- /<span class="totalPages"></span> -->
      </div>
    </section>
---


GenFit implement into the J-PARC E42 K1.8 Analyzer
====================

<div style="text-align: right;">
 2022.07.15
 </div><br>

## Environment setting

   Nothing to do. Same as K1.8-analyzer

## Features

   - GenFit package
   - GenKEK codes
   - GenKEK dsts
   - HypTPC geometry GDML

## Install

   Install the GenFit package and the GenKEK follows.

   How-to Compile:
   ```sh
   $> cp Makefile.genfit Makefile
   $> make
   $> make pcms
   ```
   You can complie seperately 1. K1.8-Analyer(k18ana) or 2. GenKEK(genfit)

   ```yml
   all: k18ana genfit
   k18ana: lib usr dst
   genfit: genkek genfit_dst
   ```

## Library

   lib/libK18Analyzer.a : for K1.8-analyzer compiling
   lib/ligGenKEK.a : for K1.8-analyzer & GenFit package & GenKEK compiling


## Paramters

   Add "TPCGDML" in the conf param file
   ```yml
   TPCGDML:  param/geometry/hypypcGeo.gdml
   ```

   And add "Fitter" and "Iteration" in the USER param file
   ```yml
   #GenFit
   Fitter 0 #KalmanFitterRefTrack
   #Fitter 1 #KalmanFitter
   #Fitter 2 #DAF w/ RefTrack
   #Fitter 3 #DAF w/o RefTrack
   nIteration 5 20
   ```
