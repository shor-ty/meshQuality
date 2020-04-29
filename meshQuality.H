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

Class
    Foam::functionObjects::meshQuality

Description
    Calculates and writes out mesh quality fields to check the resulting mesh

UsmeshQuality
    postProcess -func meshQuality

SourceFiles
    meshQuality.C

\*---------------------------------------------------------------------------*/

#ifndef meshQuality_H
#define meshQuality_H

#include "fvMeshFunctionObject.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                               Class meshQuality Declaration
\*---------------------------------------------------------------------------*/

class meshQuality
:
    public fvMeshFunctionObject
{
    // Private Data
 
        //- Switch to write the volume of the single cells
        Switch writeVolume_;
 
        //- Switch to write the cell types of the single cells
        Switch writeCellTypes_;
 
        //- Switch to write the mesh (cell) non-orthogonality
        Switch writeCellNonOrthogonality_;
 
        //- Switch to write the mesh (cell) skewness
        Switch writeCellSkewness_;
 
        //- Switch to write the mesh (face) non-orthogonality
        Switch writeFaceNonOrthogonality_;
 
        //- Switch to write the mesh (face) skewness
        Switch writeFaceSkewness_;

public:

    //- Runtime type information
    TypeName("meshQuality");


    // Constructors

        //- Construct from Time and dictionary
        meshQuality
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );


    //- Destructor
    virtual ~meshQuality();


    // Member Functions

        //- Read the data
        virtual bool read(const dictionary&);

        //- Execute
        virtual bool execute();

        //- Write
        virtual bool write();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
