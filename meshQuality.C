/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2018-2019 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "meshQuality.H"
#include "addToRunTimeSelectionTable.H"

#include "hexMatcher.H"
#include "wedgeMatcher.H"
#include "prismMatcher.H"
#include "pyrMatcher.H"
#include "tetWedgeMatcher.H"
#include "tetMatcher.H"
#include "cellQuality.H"

#include "surfaceFields.H"
#include "primitiveMeshTools.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(meshQuality, 0);
    addToRunTimeSelectionTable(functionObject, meshQuality, dictionary);
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::meshQuality::meshQuality
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fvMeshFunctionObject(name, runTime, dict),
    writeVolume_(false),
    writeCellTypes_(false),
    writeCellNonOrthogonality_(false),
    writeCellSkewness_(false),
    writeFaceNonOrthogonality_(false),
    writeFaceSkewness_(false)
{
    read(dict);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::functionObjects::meshQuality::~meshQuality()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::meshQuality::read(const dictionary& dict)
{
    writeVolume_ = dict.lookupOrDefault<Switch>("writeVolume", "yes");
    writeCellTypes_ = dict.lookupOrDefault<Switch>("writeCellTypes", "yes");
    writeCellNonOrthogonality_ =
        dict.lookupOrDefault<Switch>("writeCellNonOrthogonality", "yes");
    writeCellSkewness_ =
        dict.lookupOrDefault<Switch>("writeCellSkewness", "yes");
    writeFaceNonOrthogonality_ =
        dict.lookupOrDefault<Switch>("writeFaceNonOrthogonality", "yes");
    writeFaceSkewness_ =
        dict.lookupOrDefault<Switch>("writeFaceSkewness", "yes");

    return true;
}


bool Foam::functionObjects::meshQuality::execute()
{
    if (writeVolume_)
    {
        tmp<volScalarField> tvolume 
        (
            new volScalarField
            (
                IOobject
                (
                    "meshVolume",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE,
                    false
                ),
                mesh_,
                dimensionedScalar(dimVol, 0),
                word("zeroGradient")
            )
        );

        scalarField& volume = tvolume.ref();
        volume = mesh_.V();

        writeVolume_ = store(tvolume);
    }

    if (writeCellTypes_)
    {
        tmp<volScalarField> tcellType
        (
            new volScalarField
            (
                IOobject
                (
                    "meshCellType",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE,
                    false
                ),
                mesh_,
                dimensionedScalar(dimless, -1),
                word("zeroGradient")
            )
        );

        scalarField& cellType = tcellType.ref();

        // Construct shape recognizers
        hexMatcher hex;
        prismMatcher prism;
        wedgeMatcher wedge;
        pyrMatcher pyr;
        tetWedgeMatcher tetWedge;
        tetMatcher tet;

        // Definition for shapes
        // -1 = unknown (default)
        //  0 = hexahedral
        //  1 = tetrahedral
        //  2 = polyhedral
        //  3 = pyramid
        //  4 = prism
        //  5 = wedges
        //  6 = tetWedges
         
        for (label celli = 0; celli < mesh_.nCells(); celli++)
        {
            if (hex.isA(mesh_, celli))
            {
                cellType[celli] = 0;
            }
            else if (tet.isA(mesh_, celli))
            {
                cellType[celli] = 1;
            }
            else if (pyr.isA(mesh_, celli))
            {
                cellType[celli] = 3;
            }
            else if (prism.isA(mesh_, celli))
            {
                cellType[celli] = 4;
            }
            else if (wedge.isA(mesh_, celli))
            {
                cellType[celli] = 5;
            }
            else if (tetWedge.isA(mesh_, celli))
            {
                cellType[celli] = 6;
            }
            else
            {
                cellType[celli] = 3;
            }
        }
        
        writeCellTypes_ = store(tcellType);
    }

    // Build cell quality object
    const cellQuality& cQ = mesh_;

    if (writeCellNonOrthogonality_)
    {
        tmp<volScalarField> tnonOrthogonality
        (
            new volScalarField 
            (
                IOobject
                (
                    "meshCellNonOrthogonality",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE,
                    false
                ),
                mesh_,
                dimensionedScalar(dimless, 0)
            )
        );

        scalarField& nonOrthogonality = tnonOrthogonality.ref();
        nonOrthogonality = cQ.nonOrthogonality();
        writeCellNonOrthogonality_ = store(tnonOrthogonality);
    }

    if (writeCellSkewness_)
    {
        tmp<volScalarField> tskewness
        (
            new volScalarField 
            (
                IOobject
                (
                    "meshSkewness",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE,
                    false
                ),
                mesh_,
                dimensionedScalar(dimless, 0)
            )
        );

        scalarField& skewness = tskewness.ref();
        skewness = cQ.skewness();
        writeCellSkewness_ = store(tskewness);
    }

    if (writeFaceNonOrthogonality_)
    {
        tmp<surfaceScalarField> tfnonOrthogonality
        (
            new surfaceScalarField 
            (
                IOobject
                (
                    "meshFaceNonOrthogonality",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE,
                    false
                ),
                mesh_,
                dimensionedScalar(dimless, 0)
            )
        );


        scalarField& fnonOrthogonality = tfnonOrthogonality.ref();
        fnonOrthogonality = cQ.faceNonOrthogonality();

        writeFaceNonOrthogonality_ = store(tfnonOrthogonality);
    }
 
    if (writeFaceSkewness_)
    {
        tmp<surfaceScalarField> tfSkewness
        (
            new surfaceScalarField 
            (
                IOobject
                (
                    "meshFaceSkewness",
                    mesh_.time().timeName(),
                    mesh_,
                    IOobject::NO_READ,
                    IOobject::AUTO_WRITE,
                    false
                ),
                mesh_,
                dimensionedScalar(dimless, 0)
            )
        );


        scalarField& fSkewness = tfSkewness.ref();
        fSkewness = cQ.faceSkewness();

        writeFaceSkewness_ = store(tfSkewness);
    }

    return
    (
        writeVolume_
     && writeCellTypes_
     && writeCellNonOrthogonality_
     && writeCellSkewness_ 
     && writeFaceNonOrthogonality_
     && writeFaceSkewness_ 
    );
}


bool Foam::functionObjects::meshQuality::write()
{
    if (writeVolume_)
    {
        Info<< "Writing mesh volume field" << endl;
        writeVolume_ = writeObject("meshVolume");
    }

    if (writeCellTypes_)
    {
        Info<< "Writing mesh cell types field" << endl;
            //<< "Definition:\n -1 = unknown shape\n  0 = hexahedral\n"
            //<< "  1 = tetrahedral\n  2 = polyhedral\n  3 = pyramid\n"
            //<< "  4 = prism\n  5 = wedges\n  6 = tetWedges" << endl;

        writeCellTypes_ = writeObject("meshCellType");
    }

    if (writeCellNonOrthogonality_)
    {
        Info<< "Writing mesh cell non-orthogonality field" << endl;
        writeCellNonOrthogonality_ = writeObject("meshCellNonOrthogonality");
    }

    if (writeCellSkewness_)
    {
        Info<< "Writing mesh cell skewness field" << endl;
        writeCellSkewness_ = writeObject("meshCellSkewness");
    }

    if (writeFaceNonOrthogonality_)
    {
        Info<< "Writing mesh (face) non-orthogonality field" << endl;
        writeFaceNonOrthogonality_ = writeObject("meshFaceNonOrthogonality");
    }

    if (writeFaceSkewness_)
    {
        Info<< "Writing mesh (face) skewness field" << endl;
        writeFaceSkewness_ = writeObject("meshFaceSkewness");
    }

    return
    (
        writeVolume_
     && writeCellTypes_
     && writeCellNonOrthogonality_
     && writeCellSkewness_
     && writeFaceNonOrthogonality_
     && writeFaceSkewness_
    );
}


// ************************************************************************* //
