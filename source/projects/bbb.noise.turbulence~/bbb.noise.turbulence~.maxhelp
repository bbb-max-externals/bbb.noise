{
  "patcher" : {
    "fileversion" : 1,
    "appversion" : {
      "major" : 8,
      "minor" : 6,
      "revision" : 4
    },
    "classnamespace" : "box",
    "rect" : [100.0, 100.0, 640.0, 810.0],
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
    "description" : "",
    "digest" : "",
    "tags" : "",
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
          "patching_rect" : [50.0, 30.0, 250.0, 20.0],
          "text" : "bbb.noise.turbulence~"
        }
      },
      {
        "box" : {
          "id" : "obj-2",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 55.0, 410.0, 20.0],
          "text" : "Turbulence (abs-sum fractal) noise signal generator (MC support)"
        }
      },
      {
        "box" : {
          "id" : "obj-3",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 95.0, 370.0, 20.0],
          "text" : "inlet 0: (signal) x / messages (seed, randomize)"
        }
      },
      {
        "box" : {
          "id" : "obj-4",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 115.0, 480.0, 20.0],
          "text" : "inlet 1: (signal) y  |  inlet 2: (signal) z  |  inlet 3: (signal) w"
        }
      },
      {
        "box" : {
          "id" : "obj-5",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 135.0, 250.0, 20.0],
          "text" : "outlet 0: (signal) noise value"
        }
      },
      {
        "box" : {
          "id" : "obj-6",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 185.0, 150.0, 20.0],
          "text" : "--- Example ---"
        }
      },
      {
        "box" : {
          "id" : "obj-7",
          "maxclass" : "newobj",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : ["signal"],
          "patching_rect" : [50.0, 225.0, 80.0, 22.0],
          "text" : "cycle~ 0.1"
        }
      },
      {
        "box" : {
          "id" : "obj-8",
          "maxclass" : "newobj",
          "numinlets" : 4,
          "numoutlets" : 1,
          "outlettype" : ["signal"],
          "patching_rect" : [50.0, 285.0, 240.0, 22.0],
          "text" : "bbb.noise.turbulence~ @input_dim 1"
        }
      },
      {
        "box" : {
          "id" : "obj-9",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [330.0, 270.0, 70.0, 22.0],
          "text" : "seed 42"
        }
      },
      {
        "box" : {
          "id" : "obj-10",
          "maxclass" : "message",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : [""],
          "patching_rect" : [330.0, 300.0, 70.0, 22.0],
          "text" : "randomize"
        }
      },
      {
        "box" : {
          "id" : "obj-11",
          "maxclass" : "newobj",
          "numinlets" : 2,
          "numoutlets" : 1,
          "outlettype" : ["signal"],
          "patching_rect" : [50.0, 345.0, 50.0, 22.0],
          "text" : "*~ 0.5"
        }
      },
      {
        "box" : {
          "id" : "obj-12",
          "maxclass" : "newobj",
          "numinlets" : 1,
          "numoutlets" : 2,
          "outlettype" : ["signal", "signal"],
          "patching_rect" : [50.0, 405.0, 80.0, 22.0],
          "text" : "live.gain~"
        }
      },
      {
        "box" : {
          "id" : "obj-13",
          "maxclass" : "newobj",
          "numinlets" : 2,
          "numoutlets" : 0,
          "outlettype" : [],
          "patching_rect" : [50.0, 465.0, 50.0, 22.0],
          "text" : "ezdac~"
        }
      },
      {
        "box" : {
          "id" : "obj-14",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 525.0, 150.0, 20.0],
          "text" : "--- Attributes ---"
        }
      },
      {
        "box" : {
          "id" : "obj-15",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 550.0, 450.0, 20.0],
          "text" : "@input_dim (int, default: 2, range: 1-4) — Number of input dimensions"
        }
      },
      {
        "box" : {
          "id" : "obj-16",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 575.0, 450.0, 20.0],
          "text" : "@signed (int, default: 0) — Bipolar(1) or unipolar(0) output"
        }
      },
      {
        "box" : {
          "id" : "obj-17",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 600.0, 450.0, 20.0],
          "text" : "@seed (int, default: 0) — Random seed (deterministic)"
        }
      },
      {
        "box" : {
          "id" : "obj-18",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 625.0, 450.0, 20.0],
          "text" : "@scale (float, default: 1.0) — Coordinate scale factor"
        }
      },
      {
        "box" : {
          "id" : "obj-19",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 650.0, 450.0, 20.0],
          "text" : "@offset (float, default: 0.0) — Coordinate offset"
        }
      },
      {
        "box" : {
          "id" : "obj-20",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 675.0, 500.0, 20.0],
          "text" : "@source (symbol, default: simplex) — Basis noise: perlin / simplex / value"
        }
      },
      {
        "box" : {
          "id" : "obj-21",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 700.0, 450.0, 20.0],
          "text" : "@octaves (int, default: 4, range: 1-8) — Number of fractal layers"
        }
      },
      {
        "box" : {
          "id" : "obj-22",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 725.0, 450.0, 20.0],
          "text" : "@gain (float, default: 0.5, range: 0-1) — Amplitude decay per octave"
        }
      },
      {
        "box" : {
          "id" : "obj-23",
          "maxclass" : "comment",
          "numinlets" : 1,
          "numoutlets" : 0,
          "patching_rect" : [50.0, 750.0, 450.0, 20.0],
          "text" : "@lacunarity (float, default: 2.0) — Frequency multiplier per octave"
        }
      }
    ],
    "lines" : [
      {
        "patchline" : {
          "source" : ["obj-7", 0],
          "destination" : ["obj-8", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-8", 0],
          "destination" : ["obj-11", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-9", 0],
          "destination" : ["obj-8", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-10", 0],
          "destination" : ["obj-8", 0]
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
          "source" : ["obj-12", 0],
          "destination" : ["obj-13", 0]
        }
      },
      {
        "patchline" : {
          "source" : ["obj-12", 1],
          "destination" : ["obj-13", 1]
        }
      }
    ]
  }
}
