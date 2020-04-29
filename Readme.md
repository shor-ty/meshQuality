### Mesh Quality Function Object ###

* This repository includes the meshQuality function object that can be used to write different mesh quality fields as volScalarFields or surfaceScalarFields. Thus, one can perform mesh quality analysis within Paraview.

* The function object is going to be added to the OpenFOAM Foundation dev line

## Compilation ##

In order to use the function object, you have to do the following steps:

* Load your foam environment in your shell

```bash
cd $FOAM_SRC/functionObjects/field
git clone https://shor-ty@bitbucket.org/shor-ty/meshquality.git meshQuality
cd meshQuality
```

* Now load the version you need. Replace `OpenFOAM-7.x` by your version (use tab to show the versions for which this library is available)
* Until now, only version 7 of the OpenFOAM Foundation version is supported (master branch)
* If there are other versions available, you can checkout the other ones by using

```bash
git checkout <TAB><TAB>
```

* Additionally, you have to add the source file to the Make/files file

```bash
gedit ../Make/files
```

* Now add somewhere the line (preferably at the end of the file)

```bash
meshQuality/meshQuality.C
```

* And recompile the objectFunction library using

```bash
wmake libso
```

## Usage ##
To use the function object, you can go to any OpenFOAM case (a mesh has to be available) and run:

```bash
postProcess -func meshQuality
```

If you get a message that the meshQuality dict is not found you have to create it manually. For that we first copy an existing one:

```bash
foamGet age
```

Now open the file (`system/age`) and change the type to `meshQuality` and remove the `nCorr` entry. Finally, rename the file

```bash
mv system/age system/meshQuality
```


## Parameters ##
The function object writes different fields. One can adjust them by setting the following keywords into the meshQuality file. By default, all fields are written:

 * writeCellVolume

 * writeCellType

 * writeCellNonOrthogonality

 * writeCellSkewness

 * writeFaceNonOrthogonality

 * writeFaceSkewness


## Contact ##

* If you have any questions: Tobias.Holzmann@Holzmann-cfd.com

* Website: https://Holzmann-cfd.com
