/* This file is part of the Neper software package. */
/* Copyright (C) 2003-2016, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifdef __cplusplus
extern "C"
{
#endif

/// \file neut_nodes.h
/// \brief
/// \author Romain Quey
/// \bug No known bugs

#ifndef NEUT_NODES_H
#define NEUT_NODES_H

/// \brief Free a NODES structure.
///
///
///
  extern void neut_nodes_free (struct NODES *pNodes);

/// \brief Set a NODES structure to zero.
///
///
///
  extern void neut_nodes_set_zero (struct NODES *);

/// \brief Copy a NODES structure to another NODES structure.
///
///
///
  extern void neut_nodes_memcpy (struct NODES Nodes, struct NODES *pNodes2);

/// \brief Scale the coordinates of nodes in the 3 directions of space.
///
///
///
  extern void neut_nodes_scale (struct NODES *pNodes, double scalex,
				double scaley, double scalez);

/// \brief Shift the coordinates of nodes in the 3 directions of space.
///
///
///
  extern void neut_nodes_shift (struct NODES *pNodes, double shiftx,
				double shifty, double shiftz);

/// \brief Switch two nodes.
///
///
///
  extern void neut_nodes_switch_pair (struct NODES *pNodes, int n1, int n2);

/// \brief Switch nodes.
///
///
///
  extern void neut_nodes_switch (struct NODES *pNodes, int *node_nbs);

/// \brief Reverse nodes.
///
///
///
  extern void neut_nodes_reverse (struct NODES *pNodes);

/// \brief Initialize the bounding box of nodes.
///
///
///
  extern void neut_nodes_init_bbox (struct NODES *pNodes);

/// \brief Add a node to a NODES structure.
///
///
///
  extern void neut_nodes_addnode (struct NODES *pNodes, double *NodeCoo,
				  double NodeCl);

/// \brief Project nodes along a given direction onto a Mesh.
///
///
///
  extern void neut_nodes_proj_alongontomesh (struct NODES *pN, double *n,
					     struct NODES N, struct MESH M,
					     int elset);

/// \brief Remove orphan nodes.
///
///
///
  extern int neut_nodes_rmorphans (struct NODES *pNodes, struct MESH *pMesh,
				   struct NSET *pNSet2D);

/// \brief Compute the bounding box of nodes.
///
///
///
  extern void neut_nodes_bbox (struct NODES Nodes, double *bbox);

/// \brief Get the closest node of a point.
///
///
///
  extern int neut_nodes_point_closestnode (struct NODES Nodes, double *coo,
					   int *pnode);

/// \brief Get the distance between two nodes.
///
///
///
  extern double neut_nodes_dist_pair (struct NODES Nodes, int n1, int n2);

/// \brief Get the barycenter of nodes
///
///
///
  extern void neut_nodes_bary (struct NODES Nodes, int *nodes, int
			       nodeqty, double *coo);

/// \brief Get the weighted barycenter of nodes
///
///
///
  extern void neut_nodes_wbary (struct NODES Nodes, int *nodes,
				double *nodeweights, int nodeqty,
				double *coo);

/// \brief Switch nodes of a NODES structure.
  extern void neut_nodes_renumber_switch (struct NODES *pNodes,
					  int *node_nbs);

  extern int neut_nodes_node_valid (struct NODES Nodes, int id);

  extern int neut_nodes_dim (struct NODES Nodes);

  extern void neut_nodes_centre (struct NODES Nodes, double *centre);

  extern void neut_nodes_init_nodeslave (struct NODES *pNodes);

  extern void neut_nodes_markasslave (struct NODES *pNodes, int slave, int
				      master, int *shift);

extern void neut_nodes_init_dupnodeslave (struct NODES *pNodes);

extern void neut_nodes_node_cell_dupnode (struct NODES Nodes, int node,
    int cell, int *pdupnode);

extern int neut_nodes_node_isslave (struct NODES Nodes, int node);

#endif /* NEUT_NODES_H */

#ifdef __cplusplus
}
#endif