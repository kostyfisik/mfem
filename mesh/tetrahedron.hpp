// Copyright (c) 2010, Lawrence Livermore National Security, LLC. Produced at
// the Lawrence Livermore National Laboratory. LLNL-CODE-443211. All Rights
// reserved. See file COPYRIGHT for details.
//
// This file is part of the MFEM library. For more information and source code
// availability see http://mfem.org.
//
// MFEM is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License (as published by the Free
// Software Foundation) version 2.1 dated February 1999.

#ifndef MFEM_TETRAHEDRON
#define MFEM_TETRAHEDRON

#include "../config/config.hpp"
#include "element.hpp"

namespace mfem
{

/// Data type tetrahedron element
class Tetrahedron : public Element
{
protected:
   int indices[4];

   /** The refinement flag keeps (in order) :
       1. Two marked edges given with local index (0..5) for the two faces
       that don't have the refinement edge as edge. The refinement edge
       is determined by the first two nodes of the tetrahedron. Each
       marked edge is stored in 3 bits (or as implemented in the functions
       CreateRefinementFlag and ParseRefinementFlag.
       2. Type of the element, stored in the next 3 bits.
       3. The rest is free for now. **/
   int refinement_flag;

   unsigned transform;

public:
   typedef Geometry::Constants<Geometry::TETRAHEDRON> geom_t;

   /// Constants for different types of tetrahedrons.
   enum { TYPE_PU=0, TYPE_A=1, TYPE_PF=2, TYPE_O=3, TYPE_M=4 };

   Tetrahedron() : Element(Geometry::TETRAHEDRON)
   { refinement_flag = 0; transform = 0; }

   /// Constructs tetrahedron by specifying the indices and the attribute.
   Tetrahedron(const int *ind, int attr = 1);

   /// Constructs tetrahedron by specifying the indices and the attribute.
   Tetrahedron(int ind1, int ind2, int ind3, int ind4, int attr = 1);

   /// Return element's type.
   virtual int GetType() const { return Element::TETRAHEDRON; }

   void  ParseRefinementFlag(int refinement_edges[2], int &type, int &flag);
   void CreateRefinementFlag(int refinement_edges[2], int  type, int  flag = 0);

   virtual int GetRefinementFlag() { return refinement_flag; }

   void SetRefinementFlag(int rf) { refinement_flag = rf; }

   /// Return 1 if the element needs refinement in order to get conforming mesh.
   virtual int NeedRefinement(DSTable &v_to_v, int *middle) const;

   /// Set the vertices according to the given input.
   virtual void SetVertices(const int *ind);

   /// Mark the longest edge by assuming/changing the order of the vertices.
   virtual void MarkEdge(DenseMatrix &pmat) { }

   /** Reorder the vertices so that the longest edge is from vertex 0
       to vertex 1. If called it should be once from the mesh constructor,
       because the order may be used later for setting the edges. **/
   virtual void MarkEdge(const DSTable &v_to_v, const int *length);

   virtual void ResetTransform(int tr) { transform = tr; }
   virtual unsigned GetTransform() const { return transform; }

   /// Add 'tr' to the current chain of coarse-fine transformations.
   virtual void PushTransform(int tr)
   { transform = (transform << 3) | (tr + 1); }

   /// Calculate point matrix corresponding to a chain of transformations.
   static void GetPointMatrix(unsigned transform, DenseMatrix &pm);

   /// Returns the indices of the element's  vertices.
   virtual void GetVertices(Array<int> &v) const;

   virtual int *GetVertices() { return indices; }

   virtual int GetNVertices() const { return 4; }

   virtual int GetNEdges() const { return (6); }

   virtual const int *GetEdgeVertices(int ei) const
   { return geom_t::Edges[ei]; }

   virtual int GetNFaces(int &nFaceVertices) const
   { nFaceVertices = 3; return 4; }

   virtual const int *GetFaceVertices(int fi) const
   { MFEM_ABORT("not implemented"); return NULL; }

   virtual Element *Duplicate(Mesh *m) const;

   virtual ~Tetrahedron() { }
};

extern Linear3DFiniteElement TetrahedronFE;

}

#endif
