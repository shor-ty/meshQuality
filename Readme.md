### Mesh Quality Function Object ###

* This repository includes the meshQuality function object that can be used to write different mesh quality fields as volScalarFields or surfaceScalarFields. Thus, one can perform mesh quality analysis within Paraview.

* The function object is going to be added to the OpenFOAM Foundation dev line

## Compilation ##

In order to use the function object, you have to do the following steps:
> Load your foam environment in your shell
> cd $FOAM_SRC/functionObjects/fields
> git clone https://shor-ty@bitbucket.org/shor-ty/meshquality.git meshQuality
> cd meshQuality
Now you have to get the version you need. Replace OpenFOAM-7.x by your version (use tab to show the versions for which this library is available)
> git checkout OpenFOAM-7.x
Additionally, you have to add the source file to the Make/files file
> gedit ../Make/files
Now add somewhere the line
> meshQuality/meshQuality.C
And recompile the library using
> wmake libso

## Usage ##
To use the function object, you can go to any case file (a mesh has to be available) and run:
> postProcess -func meshQuality
If you get a message that the meshQuality dict is not found you have to do the following:
> foamGet meshQuality
Now it should work


## Parameters ##
The function object writes different fields. One can adjust them by setting the following keywords into the meshQuality file (foamGet)
 * writeCellVolume
 * writeCellType
 * writeCellNonOrthogonality
 * writeCellSkewness
 * writeFaceNonOrthogonality
 * writeFaceSkewness


## Contact ##

If you have any questions: Tobias.Holzmann@Holzmann-cfd.de
Website: https://Holzmann-cfd.com


