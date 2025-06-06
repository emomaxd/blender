/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

/** \file
 * \ingroup freestyle
 * \brief Classes to define an Extended Winged Edge data structure.
 */

#include "Curvature.h"
#include "Nature.h"
#include "WEdge.h"

#include "MEM_guardedalloc.h"

namespace Freestyle {

typedef Nature::EdgeNature WXNature;

/**********************************
 *                                *
 *                                *
 *             WXVertex           *
 *                                *
 *                                *
 **********************************/

class WXVertex : public WVertex {
 private:
  // Curvature info
  CurvatureInfo *_curvatures;

 public:
  inline WXVertex(const Vec3f &v) : WVertex(v)
  {
    _curvatures = nullptr;
  }

  /** Copy constructor */
  WXVertex(WXVertex &iBrother) : WVertex(iBrother)
  {
    _curvatures = new CurvatureInfo(*iBrother._curvatures);
  }

  virtual WVertex *duplicate()
  {
    WXVertex *clone = new WXVertex(*this);
    return clone;
  }

  virtual ~WXVertex()
  {
    if (_curvatures) {
      delete _curvatures;
    }
  }

  virtual void Reset()
  {
    if (_curvatures) {
      _curvatures->Kr = 0.0;
    }
  }

  inline void setCurvatures(CurvatureInfo *ci)
  {
    _curvatures = ci;
  }

  inline bool isFeature();

  inline CurvatureInfo *curvatures()
  {
    return _curvatures;
  }

  MEM_CXX_CLASS_ALLOC_FUNCS("Freestyle:WXVertex")
};

/**********************************
 *                                *
 *                                *
 *             WXEdge             *
 *                                *
 *                                *
 **********************************/

class WXEdge : public WEdge {
 private:
  // flag to indicate whether the edge is a silhouette edge or not
  WXNature _nature;
  // 0: the order doesn't matter. 1: the order is the original one. -1: the order is not good
  short _order;
  // A front facing edge is an edge for which the bording face which is the nearest from the
  // viewpoint is front. A back facing edge is the opposite.
  bool _front;

 public:
  inline WXEdge() : WEdge()
  {
    _nature = Nature::NO_FEATURE;
    _front = false;
    _order = 0;
  }

  inline WXEdge(WOEdge *iOEdge) : WEdge(iOEdge)
  {
    _nature = Nature::NO_FEATURE;
    _front = false;
    _order = 0;
  }

  inline WXEdge(WOEdge *iaOEdge, WOEdge *ibOEdge) : WEdge(iaOEdge, ibOEdge)
  {
    _nature = Nature::NO_FEATURE;
    _front = false;
    _order = 0;
  }

  /** Copy constructor */
  inline WXEdge(WXEdge &iBrother) : WEdge(iBrother)
  {
    _nature = iBrother.nature();
    _front = iBrother._front;
    _order = iBrother._order;
  }

  virtual WEdge *duplicate()
  {
    WXEdge *clone = new WXEdge(*this);
    return clone;
  }

  virtual ~WXEdge() {}

  virtual void Reset()
  {
    _nature = _nature & ~Nature::SILHOUETTE;
    _nature = _nature & ~Nature::SUGGESTIVE_CONTOUR;
  }

  /** accessors */
  inline WXNature nature()
  {
    return _nature;
  }

  inline bool front()
  {
    return _front;
  }

  inline short order() const
  {
    return _order;
  }

  /** modifiers */
  inline void setFront(bool iFront)
  {
    _front = iFront;
  }

  inline void setNature(WXNature iNature)
  {
    _nature = iNature;
  }

  inline void AddNature(WXNature iNature)
  {
    _nature = _nature | iNature;
  }

  inline void setOrder(int i)
  {
    _order = i;
  }

  MEM_CXX_CLASS_ALLOC_FUNCS("Freestyle:WXEdge")
};

/**********************************
 *                                *
 *                                *
 *             WXFace             *
 *                                *
 *                                *
 **********************************/

/** Class to store a smooth edge (i.e Hertzman & Zorin smooth silhouette edges) */
class WXSmoothEdge {
 public:
  typedef ushort Configuration;
  static const Configuration EDGE_EDGE = 1;
  static const Configuration VERTEX_EDGE = 2;
  static const Configuration EDGE_VERTEX = 3;

  WOEdge *_woea;  // Oriented edge from which the silhouette edge starts
  WOEdge *_woeb;  // Oriented edge where the silhouette edge ends
  float _ta;  // The silhouette starting point's coordinates are : _woea[0]+ta*(_woea[1]-_woea[0])
  float _tb;  // The silhouette ending point's coordinates are : _woeb[0]+ta*(_woeb[1]-_woeb[0])
  bool _front;
  Configuration _config;

  WXSmoothEdge()
  {
    _woea = nullptr;
    _woeb = nullptr;
    _ta = 0.0f;
    _tb = 0.0f;
    _front = false;
    _config = EDGE_EDGE;
  }

  WXSmoothEdge(const WXSmoothEdge &iBrother)
  {
    _woea = iBrother._woea;
    _woeb = iBrother._woeb;
    _ta = iBrother._ta;
    _tb = iBrother._tb;
    _config = iBrother._config;
    _front = iBrother._front;
  }

  ~WXSmoothEdge() {}

  inline WOEdge *woea()
  {
    return _woea;
  }

  inline WOEdge *woeb()
  {
    return _woeb;
  }

  inline float ta() const
  {
    return _ta;
  }

  inline float tb() const
  {
    return _tb;
  }

  inline bool front() const
  {
    return _front;
  }

  inline Configuration configuration() const
  {
    return _config;
  }

  /** modifiers */
  inline void setWOeA(WOEdge *iwoea)
  {
    _woea = iwoea;
  }

  inline void setWOeB(WOEdge *iwoeb)
  {
    _woeb = iwoeb;
  }

  inline void setTa(float ta)
  {
    _ta = ta;
  }

  inline void setTb(float tb)
  {
    _tb = tb;
  }

  inline void setFront(bool iFront)
  {
    _front = iFront;
  }

  inline void setConfiguration(Configuration iConf)
  {
    _config = iConf;
  }

  MEM_CXX_CLASS_ALLOC_FUNCS("Freestyle:WXSmoothEdge")
};

/* Class to store a value per vertex and a smooth edge.
 * The WXFace stores a list of these
 */
class WXFace;

class WXFaceLayer {
 public:
  void *userdata;
  WXFace *_pWXFace;
  // in case of silhouette: the values obtained when computing the normal-view direction dot
  // product. _DotP[i] is this value for the vertex i for that face.
  vector<float> _DotP;
  WXSmoothEdge *_pSmoothEdge;
  WXNature _Nature;

  // oldtmp values
  // count the number of positive dot products for vertices.
  // if this number is != 0 and !=_DotP.size() -> it is a silhouette fac
  uint _nPosDotP;

  uint _nNullDotP;  // count the number of null dot products for vertices.
  uint _ClosestPointIndex;
  bool _viewDependant;

  WXFaceLayer(WXFace *iFace, WXNature iNature, bool viewDependant)
  {
    _pWXFace = iFace;
    _pSmoothEdge = nullptr;
    _nPosDotP = 0;
    _nNullDotP = 0;
    _Nature = iNature;
    _viewDependant = viewDependant;
    userdata = nullptr;
  }

  WXFaceLayer(const WXFaceLayer &iBrother)
  {
    _pWXFace = iBrother._pWXFace;
    _pSmoothEdge = nullptr;
    _DotP = iBrother._DotP;
    _nPosDotP = iBrother._nPosDotP;
    _nNullDotP = iBrother._nNullDotP;
    _Nature = iBrother._Nature;
    if (iBrother._pSmoothEdge) {  // XXX ? It's set to null a few lines above!
      _pSmoothEdge = new WXSmoothEdge(*(iBrother._pSmoothEdge));
    }
    _viewDependant = iBrother._viewDependant;
    userdata = nullptr;
  }

  virtual ~WXFaceLayer()
  {
    if (!_DotP.empty()) {
      _DotP.clear();
    }
    if (_pSmoothEdge) {
      delete _pSmoothEdge;
      _pSmoothEdge = nullptr;
    }
  }

  inline float dotP(int i) const
  {
    return _DotP[i];
  }

  inline uint nPosDotP() const
  {
    return _nPosDotP;
  }

  inline uint nNullDotP() const
  {
    return _nNullDotP;
  }

  inline int closestPointIndex() const
  {
    return _ClosestPointIndex;
  }

  inline WXNature nature() const
  {
    return _Nature;
  }

  inline bool hasSmoothEdge() const
  {
    if (_pSmoothEdge) {
      return true;
    }
    return false;
  }

  inline WXFace *getFace()
  {
    return _pWXFace;
  }

  inline WXSmoothEdge *getSmoothEdge()
  {
    return _pSmoothEdge;
  }

  inline bool isViewDependant() const
  {
    return _viewDependant;
  }

  inline void setClosestPointIndex(int iIndex)
  {
    _ClosestPointIndex = iIndex;
  }

  inline void removeSmoothEdge()
  {
    if (!_DotP.empty()) {
      _DotP.clear();
    }
    if (_pSmoothEdge) {
      delete _pSmoothEdge;
      _pSmoothEdge = nullptr;
    }
  }

  /** If one of the face layer vertex has a DotP equal to 0, this method returns the vertex where
   * it happens */
  uint Get0VertexIndex() const;

  /** In case one of the edge of the triangle is a smooth edge, this method allows to retrieve the
   * concerned edge */
  uint GetSmoothEdgeIndex() const;

  /** retrieves the edges of the triangle for which the signs are different (a null value is not
   * considered) for the dotp values at each edge extremity
   */
  void RetrieveCuspEdgesIndices(vector<int> &oCuspEdges);

  WXSmoothEdge *BuildSmoothEdge();

  inline void setDotP(const vector<float> &iDotP)
  {
    _DotP = iDotP;
  }

  inline void PushDotP(float iDotP)
  {
    _DotP.push_back(iDotP);
    if (iDotP > 0.0f) {
      ++_nPosDotP;
    }
    if (iDotP == 0.0f) { /* TODO: this comparison is weak, check if it actually works. */
      ++_nNullDotP;
    }
  }

  inline void ReplaceDotP(uint index, float newDotP)
  {
    _DotP[index] = newDotP;
    updateDotPInfos();
  }

  inline void updateDotPInfos()
  {
    _nPosDotP = 0;
    _nNullDotP = 0;
    for (vector<float>::iterator d = _DotP.begin(), dend = _DotP.end(); d != dend; ++d) {
      if ((*d) > 0.0f) {
        ++_nPosDotP;
      }
      if ((*d) == 0.0f) { /* TODO: ditto. */
        ++_nNullDotP;
      }
    }
  }

  MEM_CXX_CLASS_ALLOC_FUNCS("Freestyle:WXFaceLayer")
};  // namespace Freestyle

class WXFace : public WFace {
 protected:
  Vec3f _center;  // center of the face
  float _Z;       // distance from viewpoint to the center of the face
  bool _front;    // flag to tell whether the face is front facing or back facing
  float _dotp;    // value obtained when computing the normal-viewpoint dot product

  vector<WXFaceLayer *> _SmoothLayers;  // The data needed to store one or several smooth edges
                                        // that traverse the face

 public:
  inline WXFace() : WFace()
  {
    _Z = 0.0f;
    _front = false;
  }

  /** Copy constructor */
  WXFace(WXFace &iBrother) : WFace(iBrother)
  {
    _center = iBrother.center();
    _Z = iBrother.Z();
    _front = iBrother.front();
    for (vector<WXFaceLayer *>::iterator wxf = iBrother._SmoothLayers.begin(),
                                         wxfend = iBrother._SmoothLayers.end();
         wxf != wxfend;
         ++wxf)
    {
      _SmoothLayers.push_back(new WXFaceLayer(**wxf));
    }
  }

  virtual WFace *duplicate()
  {
    WXFace *clone = new WXFace(*this);
    return clone;
  }

  virtual ~WXFace()
  {
    if (!_SmoothLayers.empty()) {
      for (vector<WXFaceLayer *>::iterator wxf = _SmoothLayers.begin(),
                                           wxfend = _SmoothLayers.end();
           wxf != wxfend;
           ++wxf)
      {
        delete (*wxf);
      }
      _SmoothLayers.clear();
    }
  }

  /** designed to build a specialized WEdge for use in MakeEdge */
  virtual WEdge *instanciateEdge() const
  {
    return new WXEdge;
  }

  /** accessors */
  inline Vec3f &center()
  {
    return _center;
  }

  inline float Z()
  {
    return _Z;
  }

  inline bool front()
  {
    return _front;
  }

  inline float dotp()
  {
    return _dotp;
  }

  inline bool hasSmoothEdges() const
  {
    for (vector<WXFaceLayer *>::const_iterator wxf = _SmoothLayers.begin(),
                                               wxfend = _SmoothLayers.end();
         wxf != wxfend;
         ++wxf)
    {
      if ((*wxf)->hasSmoothEdge()) {
        return true;
      }
    }
    return false;
  }

  vector<WXFaceLayer *> &getSmoothLayers()
  {
    return _SmoothLayers;
  }

  /** retrieve the smooth edges that match the Nature given as argument */
  void retrieveSmoothEdges(WXNature iNature, vector<WXSmoothEdge *> &oSmoothEdges)
  {
    for (vector<WXFaceLayer *>::iterator wxf = _SmoothLayers.begin(), wxfend = _SmoothLayers.end();
         wxf != wxfend;
         ++wxf)
    {
      if ((*wxf)->hasSmoothEdge() && ((*wxf)->_Nature & iNature)) {
        oSmoothEdges.push_back((*wxf)->_pSmoothEdge);
      }
    }
  }

  void retrieveSmoothEdgesLayers(WXNature iNature, vector<WXFaceLayer *> &oSmoothEdgesLayers)
  {
    for (vector<WXFaceLayer *>::iterator wxf = _SmoothLayers.begin(), wxfend = _SmoothLayers.end();
         wxf != wxfend;
         ++wxf)
    {
      if ((*wxf)->hasSmoothEdge() && ((*wxf)->_Nature & iNature)) {
        oSmoothEdgesLayers.push_back((*wxf));
      }
    }
  }

  void retrieveSmoothLayers(WXNature iNature, vector<WXFaceLayer *> &oSmoothLayers)
  {
    for (vector<WXFaceLayer *>::iterator wxf = _SmoothLayers.begin(), wxfend = _SmoothLayers.end();
         wxf != wxfend;
         ++wxf)
    {
      if ((*wxf)->_Nature & iNature) {
        oSmoothLayers.push_back(*wxf);
      }
    }
  }

  /** modifiers */
  inline void setCenter(const Vec3f &iCenter)
  {
    _center = iCenter;
  }

  void ComputeCenter();

  inline void setZ(float z)
  {
    _Z = z;
  }

  inline void setFront(bool iFront)
  {
    _front = iFront;
  }

  inline void setDotP(float iDotP)
  {
    _dotp = iDotP;
    if (_dotp > 0.0f) {
      _front = true;
    }
    else {
      _front = false;
    }
  }

  inline void AddSmoothLayer(WXFaceLayer *iLayer)
  {
    _SmoothLayers.push_back(iLayer);
  }

  inline void Reset()
  {
    vector<WXFaceLayer *> layersToKeep;
    for (vector<WXFaceLayer *>::iterator wxf = _SmoothLayers.begin(), wxfend = _SmoothLayers.end();
         wxf != wxfend;
         ++wxf)
    {
      if ((*wxf)->isViewDependant()) {
        delete (*wxf);
      }
      else {
        layersToKeep.push_back(*wxf);
      }
    }
    _SmoothLayers = layersToKeep;
  }

  /** Clears everything */
  inline void Clear()
  {
    for (vector<WXFaceLayer *>::iterator wxf = _SmoothLayers.begin(), wxfend = _SmoothLayers.end();
         wxf != wxfend;
         ++wxf)
    {
      delete (*wxf);
    }
    _SmoothLayers.clear();
  }

  virtual void ResetUserData()
  {
    WFace::ResetUserData();
    for (vector<WXFaceLayer *>::iterator wxf = _SmoothLayers.begin(), wxfend = _SmoothLayers.end();
         wxf != wxfend;
         ++wxf)
    {
      (*wxf)->userdata = nullptr;
    }
  }

  MEM_CXX_CLASS_ALLOC_FUNCS("Freestyle:WXFace")
};

/**********************************
 *                                *
 *                                *
 *             WXShape            *
 *                                *
 *                                *
 **********************************/

class WXShape : public WShape {
#if 0
 public:
  typedef WXShape type_name;
#endif

 protected:
  bool _computeViewIndependent;  // flag to indicate whether the view independent stuff must be
                                 // computed or not

 public:
  inline WXShape() : WShape()
  {
    _computeViewIndependent = true;
  }

  /** copy constructor */
  inline WXShape(WXShape &iBrother) : WShape(iBrother)
  {
    _computeViewIndependent = iBrother._computeViewIndependent;
  }

  virtual WShape *duplicate()
  {
    WXShape *clone = new WXShape(*this);
    return clone;
  }

  virtual ~WXShape() {}

  inline bool getComputeViewIndependentFlag() const
  {
    return _computeViewIndependent;
  }

  inline void setComputeViewIndependentFlag(bool iFlag)
  {
    _computeViewIndependent = iFlag;
  }

  /** designed to build a specialized WFace for use in MakeFace */
  virtual WFace *instanciateFace() const
  {
    return new WXFace;
  }

  /**
   * Adds a new face to the shape returns the built face.
   * - iVertexList
   *   List of face's vertices.
   *   These vertices are not added to the WShape vertex list; they are
   *   supposed to be already stored when calling MakeFace. The order in which the vertices are
   *   stored in the list determines the face's edges orientation and (so) the face orientation.
   */
  virtual WFace *MakeFace(vector<WVertex *> &iVertexList,
                          vector<bool> &iFaceEdgeMarksList,
                          uint iMaterialIndex);

  /**
   * Adds a new face to the shape.
   * The difference with the previous method is that this one is designed to build a WingedEdge
   * structure for which there are per vertex normals, opposed to per face normals.
   * returns the built face.
   *
   * - iVertexList:
   *   List of face's vertices. These vertices are not added to the WShape vertex list;
   *   they are supposed to be already stored when calling MakeFace.
   *   The order in which the vertices are stored in the list determines
   *   the face's edges orientation and (so) the face orientation.
   * - iNormalsList:
   *   The list of normals, iNormalsList[i]
   *   corresponding to the normal of the vertex iVertexList[i] for that face.
   * - iTexCoordsList:
   *   The list of texture coords, iTexCoordsList[i]
   *   corresponding to the normal of the vertex iVertexList[i] for that face.
   */
  virtual WFace *MakeFace(vector<WVertex *> &iVertexList,
                          vector<Vec3f> &iNormalsList,
                          vector<Vec2f> &iTexCoordsList,
                          vector<bool> &iFaceEdgeMarksList,
                          uint iMaterialIndex);

  /** Reset all edges and vertices flags (which might have been set up on a previous pass) */
  virtual void Reset()
  {
    // Reset Edges
    vector<WEdge *> &wedges = getEdgeList();
    for (vector<WEdge *>::iterator we = wedges.begin(), weend = wedges.end(); we != weend; ++we) {
      ((WXEdge *)(*we))->Reset();
    }

    // Reset faces:
    vector<WFace *> &wfaces = GetFaceList();
    for (vector<WFace *>::iterator wf = wfaces.begin(), wfend = wfaces.end(); wf != wfend; ++wf) {
      ((WXFace *)(*wf))->Reset();
    }
  }
  /** accessors */

  MEM_CXX_CLASS_ALLOC_FUNCS("Freestyle:WXShape")
};

/*
 * #############################################
 * #############################################
 * #############################################
 * ######                                 ######
 * ######   I M P L E M E N T A T I O N   ######
 * ######                                 ######
 * #############################################
 * #############################################
 * #############################################
 */
/* for inline functions */

bool WXVertex::isFeature()
{
  int counter = 0;
  vector<WEdge *> &vedges = GetEdges();
  for (vector<WEdge *>::iterator ve = vedges.begin(), vend = vedges.end(); ve != vend; ++ve) {
    if (((WXEdge *)(*ve))->nature() != Nature::NO_FEATURE) {
      counter++;
    }
  }

  if ((counter == 1) || (counter > 2)) {
    return true;
  }
  return false;
}

} /* namespace Freestyle */
