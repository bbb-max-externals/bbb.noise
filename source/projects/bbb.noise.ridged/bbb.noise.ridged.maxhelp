{
  "patcher" : {
    "fileversion" : 1,
    "appversion" : {
      "major" : 8,
      "minor" : 6,
      "revision" : 4,
      "processor" : "x86",
      "platform" : "macintel"
    },
    "classnamespace" : "box",
    "rect" : [100.0, 100.0, 600.0, 850.0],
    "bglocked" : 1,
    "openrect" : [0.0, 0.0, 0.0, 0.0],
    "openinpresentation" : 0,
    "default_fontsize" : 12.0,
    "default_fontface" : 0,
    "default_fontname" : "Arial",
    "gridonopen" : 2,
    "gridsize" : [15.0, 15.0],
    "gridsnaponopen" : 0,
    "objectsnaponopen" : 1,
    "statusbarvisible" : 2,
    "toolbarvisible" : 2,
    "lefttoolbarpinned" : 0,
    "toptoolbarpinned" : 0,
    "righttoolbarpinned" : 0,
    "bottomtoolbarpinned" : 0,
    "toolbars_unpinned_last_save" : 0,
    "tallnewobj" : 0,
    "boxanimatetime" : 200,
    "enablehscroll" : 1,
    "enablevscroll" : 1,
    "devicewidth" : 0.0,
    "description" : "Ridged multifractal noise",
    "digest" : "bbb.noise.ridged - Ridged multifractal noise generator",
    "tags" : "noise, ridged, multifractal, procedural",
    "style" : "",
    "subpatcher_template" : "",
    "assistshowspatchername" : 0,
    "boxes" : [
      {
        "box" : {
          "id" : "obj-1",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 30.0, 400.0, 20.0],
          "text" : "bbb.noise.ridged"
        }
      },
      {
        "box" : {
          "id" : "obj-2",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 55.0, 500.0, 20.0],
          "text" : "Ridged multifractal noise. Sharp ridge-like features from inverted abs."
        }
      },
      {
        "box" : {
          "id" : "obj-3",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 110.0, 150.0, 20.0],
          "text" : "--- Inlets ---"
        }
      },
      {
        "box" : {
          "id" : "obj-4",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 135.0, 350.0, 20.0],
          "text" : "inlet 0: float/list/bang (x coordinate or trigger)"
        }
      },
      {
        "box" : {
          "id" : "obj-5",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 160.0, 350.0, 20.0],
          "text" : "inlet 1: float (y coordinate)"
        }
      },
      {
        "box" : {
          "id" : "obj-6",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 200.0, 150.0, 20.0],
          "text" : "--- Outlets ---"
        }
      },
      {
        "box" : {
          "id" : "obj-7",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 225.0, 350.0, 20.0],
          "text" : "outlet 0: float (noise value)"
        }
      },
      {
        "box" : {
          "id" : "obj-8",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 270.0, 200.0, 20.0],
          "text" : "--- Basic Usage ---"
        }
      },
      {
        "box" : {
          "id" : "obj-9",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 310.0, 50.0, 22.0],
          "text" : "bang"
        }
      },
      {
        "box" : {
          "id" : "obj-10",
          "maxclass" : "number",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 370.0, 50.0, 22.0]
        }
      },
      {
        "box" : {
          "id" : "obj-11",
          "maxclass" : "newobj",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [50.0, 430.0, 150.0, 22.0],
          "text" : "bbb.noise.ridged"
        }
      },
      {
        "box" : {
          "id" : "obj-12",
          "maxclass" : "flonum",
          "numinlets" : 1,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [230.0, 430.0, 60.0, 22.0]
        }
      },
      {
        "box" : {
          "id" : "obj-13",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [170.0, 310.0, 80.0, 22.0],
          "text" : "seed 42"
        }
      },
      {
        "box" : {
          "id" : "obj-14",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [270.0, 310.0, 80.0, 22.0],
          "text" : "randomize"
        }
      },
      {
        "box" : {
          "id" : "obj-15",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 490.0, 200.0, 20.0],
          "text" : "--- Attributes ---"
        }
      },
      {
        "box" : {
          "id" : "obj-16",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 515.0, 500.0, 20.0],
          "text" : "@input_dim: number of coordinate dimensions 1-4 (default: 2)"
        }
      },
      {
        "box" : {
          "id" : "obj-17",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 540.0, 500.0, 20.0],
          "text" : "@signed: bipolar (1) or unipolar (0) output (default: 0)"
        }
      },
      {
        "box" : {
          "id" : "obj-18",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 565.0, 500.0, 20.0],
          "text" : "@seed: random seed for deterministic output (default: 0)"
        }
      },
      {
        "box" : {
          "id" : "obj-19",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 590.0, 500.0, 20.0],
          "text" : "@scale: coordinate scale factor (default: 1.0)"
        }
      },
      {
        "box" : {
          "id" : "obj-20",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 615.0, 500.0, 20.0],
          "text" : "@offset: coordinate offset (default: 0.0)"
        }
      },
      {
        "box" : {
          "id" : "obj-21",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 650.0, 500.0, 20.0],
          "text" : "@source: basis noise type - perlin/simplex/value (default: simplex)"
        }
      },
      {
        "box" : {
          "id" : "obj-22",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 675.0, 500.0, 20.0],
          "text" : "@octaves: number of fractal layers 1-8 (default: 4)"
        }
      },
      {
        "box" : {
          "id" : "obj-23",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 700.0, 500.0, 20.0],
          "text" : "@gain: amplitude decay per octave 0-1 (default: 0.5)"
        }
      },
      {
        "box" : {
          "id" : "obj-24",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 725.0, 500.0, 20.0],
          "text" : "@lacunarity: frequency multiplier per octave >0 (default: 2.0)"
        }
      }
    ],
    "lines" : [
      {
        "patchline" : {
          "source" : ["obj-9", 0],
          "destination" : ["obj-11", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-10", 0],
          "destination" : ["obj-11", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-11", 0],
          "destination" : ["obj-12", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-13", 0],
          "destination" : ["obj-11", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-14", 0],
          "destination" : ["obj-11", 0]
        }
      }
    ]
  }
}
