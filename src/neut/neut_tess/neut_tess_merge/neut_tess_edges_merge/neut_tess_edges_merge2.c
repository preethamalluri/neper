/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2019, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_edges_merge_.h"

// keepedge is a border edge (for which a vertex will be retained)
void
neut_tess_edges_merge_edgelists (struct TESS *pTess, int *edges, int edgeqty,
                                 int *pkeepedge, int **pdeledges,
                                 int *pdeledgeqty)
{
  int i, j;
  int *nedges = NULL, nedgeqty = 0;

  (*pdeledgeqty) = edgeqty;
  (*pdeledges) = ut_alloc_1d_int (*pdeledgeqty);
  ut_array_1d_int_memcpy (*pdeledges, *pdeledgeqty, edges);

  // looking for an edge which is not surrounded by deledges; this will be
  // useful to determine the edge orientation and the remaining vertices

  (*pkeepedge) = -1;
  for (i = 0; i < edgeqty; i++)
  {
    neut_tess_edge_neighedges (*pTess, edges[i], &nedges, &nedgeqty);

    for (j = 0; j < nedgeqty; j++)
      if (ut_array_1d_int_eltpos (edges, edgeqty, nedges[j]) == -1)
      {
        (*pkeepedge) = edges[i];
        break;
      }

    ut_free_1d_int_ (&nedges);

    if (*pkeepedge != -1)
      break;
  }

  if ((*pkeepedge) == -1)
    abort ();

  ut_array_1d_int_list_rmelt (pdeledges, pdeledgeqty, *pkeepedge);

  ut_free_1d_int (nedges);

  return;
}

// keepvers is in the same order than the previous vertices of keepedge
// (the edge goes in the same direction)
void
neut_tess_edges_merge_verlists (struct TESS *pTess, int *edges, int edgeqty,
                                int keepedge, int **pkeepvers, int **pdelvers,
                                int *pdelverqty)
{
  int i, j, ver, edge, veredgeqty, keepverqty, found, pos;
  int verqty, *vers = NULL;

  keepverqty = 0;
  *pdelverqty = 0;
  ut_free_1d_int_ (pkeepvers);
  ut_free_1d_int_ (pdelvers);

  // finding vertices to keep and vertices to del
  keepverqty = 0;
  (*pdelverqty) = 0;
  neut_tess_edges_vers (*pTess, edges, edgeqty, &vers, &verqty);

  for (i = 0; i < verqty; i++)
  {
    ver = vers[i];
    veredgeqty = 0;
    for (j = 0; j < (*pTess).VerEdgeQty[ver]; j++)
    {
      edge = (*pTess).VerEdgeNb[ver][j];
      if (ut_array_1d_int_eltpos (edges, edgeqty, edge) != -1)
	veredgeqty++;
    }

    if (veredgeqty == 1)
      ut_array_1d_int_list_addelt (pkeepvers, &keepverqty, ver);
    else if (veredgeqty == 2)
      ut_array_1d_int_list_addelt (pdelvers, pdelverqty, ver);
    else
      ut_error_reportbug ();
  }

  if (keepverqty != 2)
    ut_error_reportbug ();

  found = 0;
  for (i = 0; i < 2; i++)
  {
    pos = ut_array_1d_int_eltpos ((*pTess).EdgeVerNb[keepedge], 2,
                                  (*pkeepvers)[i]);
    if (pos != -1)
    {
      found = 1;
      if (i != pos)
        ut_array_1d_int_switch (*pkeepvers, 0, 1);
      break;
    }
  }

  if (!found)
    abort ();

  ut_free_1d_int (vers);

  return;
}

void
neut_tess_edges_merge_checkfaces (struct TESS *pTess, int keepedge)
{
  int i, j, keepedge2, tmp, pos, face, poly;

  for (i = 0; i < (*pTess).EdgeFaceQty[keepedge]; i++)
  {
    face = (*pTess).EdgeFaceNb[keepedge][i];

    if ((*pTess).FaceVerQty[face] < 3)
    {
      keepedge2 = (*pTess).FaceEdgeNb[face][1] != keepedge ?
                 (*pTess).FaceEdgeNb[face][1] :
                 (*pTess).FaceEdgeNb[face][2];

      int keepedgeori
        = ((*pTess).EdgeVerNb[keepedge][0] == (*pTess).EdgeVerNb[keepedge2][0])?
          1 : -1;

      // removing face from the two edges
      if (ut_array_1d_int_eltpos ((*pTess).EdgeFaceNb[keepedge],
                                  (*pTess).EdgeFaceQty[keepedge],
                                  face) == -1)
        abort ();

      neut_tess_edge_rmface (pTess, keepedge, face);

      if (ut_array_1d_int_eltpos ((*pTess).EdgeFaceNb[keepedge2],
                                  (*pTess).EdgeFaceQty[keepedge2],
                                  face) == -1)
        ut_error_reportbug ();

      neut_tess_edge_rmface (pTess, keepedge2, face);

      // removing face from its polys
      for (j = 0; j < 2; j++)
      {
        poly = (*pTess).FacePoly[face][j];
        if (poly > 0)
          neut_tess_poly_rmface (pTess, poly, face);
      }

      (*pTess).FaceState[face] = -1;

      // removing keepedge

      // removing keepedge from its vertices
      for (j = 0; j < 2; j++)
      {
        tmp = (*pTess).EdgeVerNb[keepedge][j];
        neut_tess_ver_rmedge (pTess, tmp, keepedge);
      }

      // replacing keepedge by keepedge2 in its faces
      for (j = 0; j < (*pTess).EdgeFaceQty[keepedge]; j++)
      {
        tmp = (*pTess).EdgeFaceNb[keepedge][j];

        ut_array_1d_int_list_addelt (&(*pTess).EdgeFaceNb[keepedge2],
                                     &(*pTess).EdgeFaceQty[keepedge2],
                                     tmp);

        pos = ut_array_1d_int_eltpos ((*pTess).FaceEdgeNb[tmp] + 1,
                                      (*pTess).FaceVerQty[tmp], keepedge);

        if (pos == -1)
          ut_error_reportbug ();

        (*pTess).FaceEdgeNb[tmp][pos + 1] = keepedge2;
        (*pTess).FaceEdgeOri[tmp][pos + 1] *= keepedgeori;
      }

      (*pTess).EdgeState[keepedge] = -1;
    }
  }

  return;
}
