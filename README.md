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
#GenKEK

## Environment setting

   Same as K1.8Ana

## Install the GenFit package and the GenKEK follows.

## Features
   - GenFit package()
   - GenFit-based Track Follower
   - Rave-based Vertex Finder
   - JSON-based Event Exporter (event => Event Display format)

## Install

Pre-requisite: g++11

How-to Compile:
```sh
$> make
```
## License

MIT