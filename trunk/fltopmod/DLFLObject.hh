/* $Id: DLFLObject.hh,v 1.1.1.1 2007/03/10 18:32:17 stuart Exp $ */

#ifndef _DLFL_OBJECT_HH_

#define _DLFL_OBJECT_HH_

// The Doubly Linked Face List object classs
// Contains a vertex list, edge list and a face list
// A material list is also stored for rendering purposes.
// The lists actually store pointerss

#include "DLFLCommon.hh"
#include "DLFLVertex.hh"
#include "DLFLFaceVertex.hh"
#include "DLFLEdge.hh"
#include "DLFLFace.hh"
#include "DLFLMaterial.hh"
#include "DLFLAux.hh"
#include <Graphics/Transform.hh>
#include "TMPatchFace.hh"

class DLFLObject
{
  protected :

     static uint suLastID;                             // Distinct ID for each instance

        // Generate a new unique ID
     static uint newID(void)
       {
         uint temp = suLastID;
         suLastID++;
         return temp;
       }

     static Transformation tr;                         // For doing GL transformations

  public :

     static DLFLVertexPtrArray vparray;                // For selection
     static DLFLEdgePtrArray   eparray;                // For selection
     static DLFLFacePtrArray   fparray;                // For selection
     static DLFLFaceVertexPtrArray fvparray;           // For selection
     static DLFLFacePtrArray   crustfp1;               // For Crust Modeling
     static DLFLFacePtrArray   crustfp2;               // For Crust Modeling
     static int crust_num_old_faces;                   // For Crust Modeling
     static int crust_min_face_id;                     // For Crust Modeling

        // Added by Esan
     static DLFLVertexPtrArray crustvp1;               // For ColumnModeling
     static DLFLVertexPtrArray crustvp2;               // For Column Modeling
     static int crust_num_old_verts;                   // For Column Modeling
     static int crust_min_vert_id;                     // For Column Modeling
        // End additions by Esan

     Vector3d           position;                      // Position of object
     Vector3d           scale_factor;                  // Scale of object
     Quaternion         rotation;                      // Rotation of object

  protected :

     DLFLVertexPtrList          vertex_list;           // The vertex list
     DLFLEdgePtrList            edge_list;             // The edge list
     DLFLFacePtrList            face_list;             // The face list
     DLFLMaterialPtrList        matl_list;             // Material list (for rendering)

     TMPatchFacePtrList patch_list;		// List of patch faces
     int patchsize;				 // Size of each patch
     
     uint uID;                                         // ID for this object

        // Assign a unique ID for this instance
     void assignID(void)
       {
         uID = DLFLObject :: newID();
       }

	// Free the memory allocated for the patches
     void destroyPatches()
     {
       TMPatchFacePtrList :: iterator first = patch_list.begin(), last = patch_list.end();
       TMPatchFacePtr pfp = NULL;
       while ( first != last )
	  {
	    pfp = (*first); ++first;
	    delete pfp;
	  }
       patch_list.clear();
     }

     void clearLists(void)
       {
            // Free all the pointers in the lists and clear the lists
         clear(vertex_list);
         clear(edge_list);
         clear(face_list);
         clear(matl_list);
	 destroyPatches();
       }

  public :

        // Default constructor
     DLFLObject()
       : position(), scale_factor(1), rotation(),
         vertex_list(), edge_list(), face_list(), matl_list(), patch_list(), patchsize(4)
       {
         assignID();

            // Add a default material
         matl_list.push_back(new DLFLMaterial("default",0.5,0.5,0.5));
       }

  private :

        // Copy constructor - make proper copy, don't just copy pointers
     DLFLObject(const DLFLObject& dlfl)
       : position(dlfl.position), scale_factor(dlfl.scale_factor), rotation(dlfl.rotation),
         vertex_list(dlfl.vertex_list), edge_list(dlfl.edge_list), face_list(dlfl.face_list), matl_list(dlfl.matl_list),
	 patch_list(dlfl.patch_list), patchsize(dlfl.patchsize),
         uID(dlfl.uID)
       {}

        // Assignment operator
     DLFLObject& operator = (const DLFLObject& dlfl)
       {
         position = dlfl.position;
         scale_factor = dlfl.scale_factor;
         rotation = dlfl.rotation;

            // Destroy the existing object
         clearLists();

            // Copy the lists from the new object
	 vertex_list = dlfl.vertex_list;
	 edge_list = dlfl.edge_list;
	 face_list = dlfl.face_list;
	 matl_list = dlfl.matl_list;
	 patch_list = dlfl.patch_list;
	 patchsize = dlfl.patchsize;
	 
         uID = dlfl.uID;
         return (*this);
       }

  public :

        // Destructor
     ~DLFLObject()
       {
         clearLists();
       }

        // Dump contents of this object
     void dump(ostream& o) const;

        //--- Query Functions ---//

        // Compute the genus of the mesh using Euler formula
     int genus(void) const
       {
         int v = num_vertices();
         int e = num_edges();
         int f = num_faces();
         int g = 1 - ( v - e +  f )/2;
         return g;
       }

     uint getID(void) const
       {
         return uID;
       }

     size_t num_vertices(void) const
       {
         return vertex_list.size();
       }

     size_t num_edges(void) const
       {
         return edge_list.size();
       }

     size_t num_faces(void) const
       {
         return face_list.size();
       }

     size_t num_materials(void) const
       {
         return matl_list.size();
       }

        //-- List based access to the 3 lists --//
     
     DLFLVertexPtr firstVertex(void)
       {
         return vertex_list.front();
       }

     DLFLEdgePtr firstEdge(void)
       {
         return edge_list.front();
       }

     DLFLFacePtr firstFace(void)
       {
         return face_list.front();
       }

     DLFLMaterialPtr firstMaterial(void)
       {
         return matl_list.front();
       }

     DLFLVertexPtr lastVertex(void)
       {
         return vertex_list.back();
       }

     DLFLEdgePtr lastEdge(void)
       {
         return edge_list.back();
       }

     DLFLFacePtr lastFace(void)
       {
         return face_list.back();
       }

     DLFLMaterialPtr lastMaterial(void)
       {
         return matl_list.back();
       }

     DLFLVertexPtr findVertex(uint vid);
     DLFLEdgePtr findEdge(uint eid);
     DLFLFacePtr findFace(uint fid);

     RGBColor getColor(void) const
       {
            // Return color of matl_list[0]
         return matl_list.front()->color;
       }

        //--- Access the lists through arrays ---//
     void getVertices(DLFLVertexPtrArray& vparray)
       {
         vparray.clear(); vparray.reserve(vertex_list.size());
         DLFLVertexPtrList :: const_iterator first = vertex_list.begin(), last = vertex_list.end();
         while ( first != last )
            {
              vparray.push_back(*first);
              ++first;
            }
       }

     void getEdges(DLFLEdgePtrArray& eparray)
       {
         eparray.clear(); eparray.reserve(edge_list.size());
         DLFLEdgePtrList :: const_iterator first = edge_list.begin(), last = edge_list.end();
         while ( first != last )
            {
              eparray.push_back(*first);
              ++first;
            }
       }

     void getFaces(DLFLFacePtrArray& fparray)
       {
         fparray.clear(); fparray.reserve(face_list.size());
         DLFLFacePtrList :: const_iterator first = face_list.begin(), last = face_list.end();
         while ( first != last )
            {
              fparray.push_back(*first);
              ++first;
            }
       }

     void getMaterials(DLFLMaterialPtrArray& mparray)
       {
         mparray.clear(); mparray.reserve(matl_list.size());
         DLFLMaterialPtrList :: const_iterator first = matl_list.begin(), last = matl_list.end();
         while ( first != last )
            {
              mparray.push_back(*first);
              ++first;
            }
       }

        //--- Terminal printing functions : useful for debugging ---//

     void print(void) const
       {
            // Print a summary of the DLFLObject
         cout << "Number of vertices : " << vertex_list.size() << endl;
         cout << "Number of faces : " << face_list.size() << endl;
         cout << "Number of edges : " << edge_list.size() << endl;
         cout << "Number of materials : " << matl_list.size() << endl;
         cout << "Genus : " << genus() << endl;
       }

     void printVertexList(void) const
       {
         cout << "Vertex List" << endl;
         DLFLVertexPtrList :: const_iterator first = vertex_list.begin(), last = vertex_list.end();
         while ( first != last )
            {
              (*first)->print();
              ++first;
            }
       }

     void printEdgeList(void) const
       {
         cout << "Edge List" << endl;
         DLFLEdgePtrList :: const_iterator first = edge_list.begin(), last = edge_list.end();
         while ( first != last )
            {
              (*first)->print();
              ++first;
            }
       }

     void printFaceList(void) const
       {
         cout << "Face List" << endl;

         DLFLFacePtrList :: const_iterator first = face_list.begin(), last = face_list.end();
         while ( first != last )
            {
              cout << *(*first) << endl;
              ++first;
            }
       }
     
     void printFaces(void) const
       {
         cout << "Faces" << endl;

         DLFLFacePtrList :: const_iterator first = face_list.begin(), last = face_list.end();
         while ( first != last )
            {
              (*first)->print();
              ++first;
            }
       }

  /* stuart - bezier export */
  void printPatchCVList( ) const
  {
    cout << "Patch CVs" << endl;
    TMPatchFacePtrList::const_iterator first = patch_list.begin(), last = patch_list.end();
    while( first != last ) {
      (*first)->print(cout);
      ++first;
    }
  }
     
        //--- Mutative Functions ---//

        // Reset the whole object
     void reset(void)
       {
         position.reset(); scale_factor.set(1,1,1); rotation.reset();
         clearLists();
            // Add a default material
         matl_list.push_back(new DLFLMaterial("default",0.5,0.5,0.5));
       }

     void setColor(const RGBColor& col)
       {
            // matl_list[0] is always the default material
         matl_list.front()->setColor(col);
       }

     void makeVerticesUnique(void)
       {
            // Make vertices unique
         DLFLVertexPtrList :: iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
         while ( vfirst != vlast )
            {
              (*vfirst)->makeUnique();
              ++vfirst;
            }
       }

     void makeEdgesUnique(void)
       {
            // Make edges unique
         DLFLEdgePtrList :: iterator efirst=edge_list.begin(), elast=edge_list.end();
         while ( efirst != elast )
            {
              (*efirst)->makeUnique();
              ++efirst;
            }
       }

     void makeFacesUnique(void)
       {
            // Make faces unique
         DLFLFacePtrList :: iterator ffirst=face_list.begin(), flast=face_list.end();
         while ( ffirst != flast )
            {
              (*ffirst)->makeUnique();
              ++ffirst;
            }
       }

     void makeUnique(void)
       {
         assignID();
         makeVerticesUnique();
         makeEdgesUnique();
         makeFacesUnique();
       }

     void destroy(void)
       {
            // Clear the DLFL structures
         clearLists();
       }

        // Combine two DLFLObject instances into 1 object
        // Lists of second object are cleared. Otherwise when that object is destroyed,
        // pointers in this object will become invalid.
     void splice(DLFLObject& object);

        // Reverse the orientation of all faces in the object
        // This also requires reversing all edges in the object
     void reverse(void);

     void addVertex(const DLFLVertex& vertex);         // Insert a copy
     void addVertex(DLFLVertexPtr vertexptr);          // Insert a copy
     void addVertexPtr(DLFLVertexPtr vertexptr)
       {
            // Insert the pointer.
            // **** WARNING!!! **** Pointer will be freed when list is deleted
         vertex_list.push_back(vertexptr);
       }

     void addEdge(const DLFLEdge& edge);               // Insert a copy
     void addEdge(DLFLEdgePtr edgeptr);                // Insert a copy
     void addEdgePtr(DLFLEdgePtr edgeptr)
       {
            // Insert the pointer.
            // **** WARNING!!! **** Pointer will be freed when list is deleted
         edge_list.push_back(edgeptr);
       }

     void addFace(const DLFLFace& face);               // Insert a copy
     void addFace(DLFLFacePtr faceptr);                // Insert a copy
     void addFacePtr(DLFLFacePtr faceptr)
       {
            // Insert the pointer.
            // **** WARNING!!! **** Pointer will be freed when list is deleted
         if ( faceptr->material() == NULL )
               // If Face doesn't have a material assigned to it, assign the default material
            faceptr->setMaterial(matl_list.front());
         face_list.push_back(faceptr);
       }

     void addMaterial(const DLFLMaterial& matl);       // Insert a copy
     void addMaterial(DLFLMaterialPtr matlptr);        // Insert a copy
     void addMaterialPtr(DLFLMaterialPtr matlptr)
       {
            // Insert the pointer.
            // **** WARNING!!! **** Pointer will be freed when list is deleted
         matl_list.push_back(matlptr);
       }

     DLFLVertexPtr getVertexPtr(uint index) const
       {
         if ( index >= vertex_list.size() ) return NULL;
         DLFLVertexPtrList :: const_iterator i=vertex_list.begin();
         advance(i,index);
         return (*i);
       }
     
     DLFLVertexPtr getVertexPtrID(uint id) const
       {
         DLFLVertexPtrList :: const_iterator first=vertex_list.begin(), last=vertex_list.end();
         DLFLVertexPtr vptr = NULL;
         while (first != last)
            {
              if ( (*first)->getID() == id )
                 {
                   vptr = (*first);
                   break;
                 }
              ++first;
            }
         return vptr;
       }

     void updateEdgeList(void)
       {
         DLFLEdgePtrList :: iterator first=edge_list.begin(), last=edge_list.end();
         while ( first != last )
            {
              (*first)->updateFaceVertices();
              ++first;
            }
       }

        // Check if an edge exists between two given vertices
     bool edgeExists(DLFLVertexPtr vptr1, DLFLVertexPtr vptr2);

        // Check if an edge exists between vertices given by two corners
        // Simply calls above function
     bool edgeExists(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2)
       {
         return edgeExists(fvptr1->vertex,fvptr2->vertex);
       }

        // Check if a given edge exists in the edge list. If it does pointer is set to that edge
     bool edgeExists(const DLFLEdge& e, DLFLEdgePtr& eptr)
       {
         DLFLEdgePtrList :: iterator first=edge_list.begin(), last=edge_list.end();
         while ( first != last )
            {
              if ( (*(*first)) == e )
                 {
                   eptr = *first;
                   return true;
                 }
              ++first;
            }
         eptr = NULL;
         return false;
       }

     void addEdges(DLFLEdge * edges, int num_edges)
       {
         DLFLEdgePtr eptr;
  
         for (int i=0; i < num_edges; ++i)
            {
              if ( edgeExists(edges[i],eptr) == false ) addEdge(edges[i]);
              else
                 {
                      // If Edge already exists, then the second FaceVertexPtr in the Edge must
                      // be changed to that from the new Edge with the same ID as the second one
                      // in this Edge
                   int id2 = (eptr->getFaceVertexPtr2())->getVertexID();
                   int eid1 = (edges[i].getFaceVertexPtr1())->getVertexID();

                   if (eid1 == id2)
                      eptr->setFaceVertexPtr2(edges[i].getFaceVertexPtr1());
                   else
                      eptr->setFaceVertexPtr2(edges[i].getFaceVertexPtr2());
                 }
              eptr = NULL;
            }
       }

     void addEdgesWithoutCheck(DLFLEdge * edges, int num_edges)
       {
         for (int i=0; i < num_edges; ++i)
            addEdge(edges[i]);
       }

     void updateFaceList(void)
       {
         DLFLFacePtrList :: iterator first=face_list.begin(), last=face_list.end();
         while (first != last)
            {
              (*first)->updateFacePointers();
              (*first)->addFaceVerticesToVertices();
              ++first;
            }
       }

        //-- Vertex Deletion --//
     void deleteVertex(DLFLVertexPtr vertexptr);
     void deleteVertex(uint vertex_index);

        //-- Edge Deletion --//
        // If the cleanup flag is true, any point-spheres created
        // because of the edge deletion will be removed from the object
     void deleteEdge(DLFLEdgePtr edgeptr, bool cleanup=true);
     void deleteEdge(uint edge_index, bool cleanup=true);
     

        //-- Edge Collapse --//
     void collapseEdge(DLFLEdgePtr edgeptr, bool cleanup=true);
     void collapseEdge(uint edge_index, bool cleanup=true);


        //-- Edge Insertion --//
  protected :
        /* First one is for when both corners are in the same face.
           Second one is for when both corners are in different faces.
           Neither of these functions checks is corners are cofacial or not.
           That's why they are protected
           First one uses material from parent face and is used during subdiv surfaces
           also, so the set_type flag is there.
           The second one is not used during subdiv surfaces, but needs to know which
           material to use */
     DLFLEdgePtr insertEdge1(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                             bool set_type=false);
     DLFLEdgePtr insertEdge2(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                             DLFLMaterialPtr matl=NULL);

        // Alternative versions for use in the alternate OBJ file reader
     DLFLEdgePtr insertEdgeAlt1(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                                bool set_type=false);
     DLFLEdgePtr insertEdgeAlt2(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                                DLFLMaterialPtr matl=NULL);

  public :
        /*
          The general case insertEdge subroutine. Calls one of the above, depending
          on whether the corners are cofacial or not. If the 2 corners are cofacial
          checks to see if the 2 pointers refer to the same corner, if so doesn't do insert.
        */
     DLFLEdgePtr insertEdge(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                            bool set_type=false, DLFLMaterialPtr matl=NULL);

        // Same as above but calls the alternative versions
     DLFLEdgePtr insertEdgeAlt(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                               bool set_type=false, DLFLMaterialPtr matl=NULL);

        // Same as above but doesn't do the check to see if the 2 pointers refer to the same corner
     DLFLEdgePtr insertEdgeWithoutCheck(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                                        bool set_type=false, DLFLMaterialPtr matl=NULL);

        // Splice two corners
        // Insert edge and then collapse edge
     void spliceCorners(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);

        //-- Edge cleanup --//
        // If both sides of an edge are co-planar, the edge will be removed
        // First version looks at all edges in the object
        // Second and third version looks at the specified list/array of edges
     void edgeCleanup(void);
     void edgeCleanup(const DLFLEdgePtrList& edges);
     void edgeCleanup(const DLFLEdgePtrArray& edges);

        //-- Cleanup 2-gons --//
        // Removes one of the edges in 2-gons
     void cleanup2gons(void);

        //-- Cleanup Valence 2 vertices (winged vertices) --//
        // Removes all winged vertices
     void cleanupWingedVertices(void);

        // Connect 2 half-edges (Edge,Face pair)
        // Flag indicates whether to check for existing edge between edge corners
        // Uses the version of insertEdge which checks for self loops
     void connectEdges(DLFLEdgePtr eptr1, DLFLFacePtr fptr1,
                       DLFLEdgePtr eptr2, DLFLFacePtr fptr2, bool check=true);

        // Same as above but uses the version of insertEdge which doesn't check for self loops
     void connectEdgesWithoutLoopCheck(DLFLEdgePtr eptr1, DLFLFacePtr fptr1,
                                       DLFLEdgePtr eptr2, DLFLFacePtr fptr2, bool check=true);

        // Connect 2 faces using repeated edge insertions
     void connectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
     void connectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                       int numsegs, int maxconn=-1);
	 
        // Triangularly connect 2 faces
     void dualConnectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);

        // Make connection between vertex in second face which is closest to first vertex
        // in first face
     void connectFaces(DLFLFacePtr fp1, DLFLFacePtr fp2, int numsegs=1);

        // Connect 2 faces with a bezier handle
     void bezierConnectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                             int numsegs, double wt1, double wt2);

        // Connect 2 faces with a hermite handle
     void hermiteConnectFaces(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
                              int numsegs, double wt1, double wt2,
                              int maxconn=-1, int numtwists=0);

        // Connect 3 faces by connecting closest edges between each pair of faces
     void tripleConnectFaces(DLFLFacePtr fp1, DLFLFacePtr fp2, DLFLFacePtr fp3);

        // Connect multiple faces by connecting closest edges between each pair of faces
     void multiConnectFaces(DLFLFacePtrArray fp);

        // Connect multiple faces using convex hull method. min_factor determines
        // search stop criterion for finding suitable convex hull.
     void multiConnectFaces(DLFLFacePtrArray fp, double min_factor,
                            bool make_connections = true);

        // Connect multiple faces using convex hull
     void multiConnectFaces(DLFLFacePtrArray fp, double scale_factor,
                            double extrude_dist, bool use_max_offsets=false);

        // Sub-divide each edge. Create new faces from midpoints
        // and connect the corners of the new faces which share the same midpoint
     void multiConnectMidpoints(void);

        // Scale the object (<1) and correspondingly do a scaling subdivision on each face
        // Connect the corresponding faces in the inner shell and original surface
     void multiConnectCrust(double scale_factor=0.5);
     void modifiedMultiConnectCrust(double scale_factor=0.5);
     void createSponge(double thickness, double collapse_threshold_factor=0.0);

        // Find max offset distances for each face
     void findMaxOffsets(DLFLFacePtrArray fparray, DoubleArray& maxoffsets);

        // Duplicate a given face, offseting, scaling and rotating by specified amounts
        // This is similar to extrude but no connections are made
        // A face-pair will be created. The one which is oriented in the same direction
        // of the original face will be returned
     DLFLFacePtr duplicateFace(DLFLFacePtr fptr, double offset, double rot, double sf);
     DLFLFacePtr duplicateFace(DLFLFacePtr fptr, const Vector3d& dir,
                               double offset, double rot, double sf);

        // Similar to above functions but vertices are offset in the 
        // plane of the face along the angular bisectors by specified thickness.
        // This is done instead of scaling the vertices.
        // Boolean flag indicates if thickness is absolute or fraction of edge length.
     DLFLFacePtr duplicateFacePlanarOffset(DLFLFacePtr fptr,
                                           double offset, double rot, double thickness,
                                           bool fractionalthickness);
     DLFLFacePtr duplicateFacePlanarOffset(DLFLFacePtr fptr, const Vector3d& dir,
                                           double offset, double rot, double thickness,
                                           bool fractionalthickness);

        // Extrude a face for a given distance along the normal
        // First variation is for multiple extrusions. The 'end' face will be
        // extruded given number of times.
        // Second variation allows scaling and rotation of new face w.r.t. old face
        // Pointer to the 'end' face is returned
     DLFLFacePtr extrudeFace(DLFLFacePtr fptr, double d);
     DLFLFacePtr extrudeFace(DLFLFacePtr fptr, double d, int num);
     DLFLFacePtr extrudeFace(DLFLFacePtr fptr, double d, double rot, double sf=1.0);
     DLFLFacePtr extrudeFace(DLFLFacePtr fptr, double d, int num, double rot, double sf=1.0);

        // Extrude a face for a given distance along a given direction
        // First variation is for multiple extrusions. The 'end' face will be
        // extruded given number of times.
        // Second variation allows scaling and rotation of the new face w.r.t. old face
        // Pointer to the 'end' face is returned
     DLFLFacePtr extrudeFace(DLFLFacePtr fptr, double d, const Vector3d& dir);
     DLFLFacePtr extrudeFace(DLFLFacePtr fptr, double d, const Vector3d& dir,
                             int num);
     DLFLFacePtr extrudeFace(DLFLFacePtr fptr, double d, const Vector3d& dir,
                             double rot, double sf=1.0);
     DLFLFacePtr extrudeFace(DLFLFacePtr fptr, double d, const Vector3d& dir,
                             int num, double rot, double sf=1.0);

        // Similar to above extrudes, but does planar offset of new face instead of scaling
     DLFLFacePtr extrudeFacePlanarOffset(DLFLFacePtr fptr,
                                         double d, double rot, double thickness,
                                         bool fractionalthickness);
     DLFLFacePtr extrudeFacePlanarOffset(DLFLFacePtr fptr, double d, const Vector3d& dir,
                                         double rot, double thickness,
                                         bool fractionalthickness);

     
        // Extrude a face using doo-Sabin type algorithm to compute the new vertices in the plane
        // of the old face and then move the new face as before. Twist is through a factor varying
        // from 0 to 1
     DLFLFacePtr extrudeFaceDS(DLFLFacePtr fptr, double d,
                               double twist=0.0, double sf=1.0);
     DLFLFacePtr extrudeFaceDS(DLFLFacePtr fptr, double d,
                               int num, double twist=0.0, double sf=1.0);
     DLFLFacePtr extrudeFaceDS(DLFLFacePtr fptr, double d, const Vector3d& dir,
                               double twist=0.0, double sf=1.0);
     DLFLFacePtr extrudeFaceDS(DLFLFacePtr fptr, double d, const Vector3d& dir,
                               int num, double twist=0.0, double sf=1.0);

        // Extrude the "dual" of this face. The "dual" here refers to the face which has midpoints of edges
        // of the old face as its corners. If the 'mesh' flag is true edges in the old face are deleted.
     DLFLFacePtr extrudeDualFace(DLFLFacePtr fptr, double d,
                                 double twist=0.0, double sf=1.0, bool mesh=false);
     DLFLFacePtr extrudeDualFace(DLFLFacePtr fptr, double d,
                                 int num, double twist=0.0, double sf=1.0, bool mesh=false);
     DLFLFacePtr extrudeDualFace(DLFLFacePtr fptr, double d, const Vector3d& dir,
                                 double twist=0.0, double sf=1.0, bool mesh=false);
     DLFLFacePtr extrudeDualFace(DLFLFacePtr fptr, double d, const Vector3d& dir,
                                 int num, double twist=0.0, double sf=1.0, bool mesh=false);

        // Stellation is similar to extrusion but creates a cone type structure instead of a cylindrical one
     void stellateFace(DLFLFacePtr fptr, double d);
     void stellateFace(DLFLFacePtr fptr, double d, const Vector3d& dir);

        // Double stellation - Original version by Eric. Modified by Vinod
     void doubleStellateFace(DLFLFacePtr fptr, double d);

        //--- Added by Eric ---//
     DLFLFacePtr extrudeFaceDodeca(DLFLFacePtr fptr, double d, double rot, double sf);
     DLFLFacePtr extrudeFaceDodeca(DLFLFacePtr fptr, double d, int num, double rot, double sf,
                                   bool hexagonalize=false);

        // this is just used for the hexagonalize in the dodecaface, not to be used by itself
     DLFLFacePtr extrudeHexTileFace(DLFLFacePtr fptr, double d, double rot, double sf);

     DLFLFacePtr extrudeFaceIcosa(DLFLFacePtr fptr, double d, double rot, double sf);
     DLFLFacePtr extrudeFaceIcosa(DLFLFacePtr fptr, double d, int num, double rot, double sf);

        //--- End additions by Eric ---//

  protected :

        // Extrude a face - just creates the new faces/vertices/edges and sets coordinates
        // using previously computed and stored aux-coords
     void extrudeFace(DLFLFacePtr fptr);

        // Create a new face
     void createFace(const Vector3dArray& verts, DLFLMaterialPtr matl = NULL, bool set_type=false);

        // Create a Point Sphere - a face with only 1 vertex and no edges
        // Returns the face-vertex which is created
     DLFLFaceVertexPtr createPointSphere(const Vector3d& v, DLFLMaterialPtr matl = NULL);

        // Assign flags for edge ends. Used in tile texturing - Avneet
     void assignTileTexEdgeFlags(int n=2);

  public :
     
        // Assign texture coordinates using tile textures - Avneet
     void assignTileTexCoords(int n=2);

        // FOR QUADS ONLY - Randomly assign texture coordinates from a unit square to the 4 corners
     void randomAssignTexCoords(void);

        //-- Edge Subdivision --//
        // Return pointer to the newly added vertex 
     DLFLVertexPtr subDivideEdge(DLFLEdgePtr edgeptr, bool set_type=false);
     DLFLVertexPtr subDivideEdge(uint edge_index);
     
        // Planarize
     void planarize(void);

        // Spheralize
     void spheralize(void);

        // Smoothing the mesh
     void meshsmooth(void);

        // Subdivide into multiple edges
     void subDivideEdge(int num_divs, DLFLEdgePtr edgeptr, bool set_type=false);
     void subDivideEdge(int num_divs, uint edge_index);

        // Trisect an edge - new points will be calculated based on scale factor
        // with respect to mid point of edge OR offset from end points
        // Boolean flag indicates if specified number is a scale factor or offset distance
        // Offset distance is clamped to lie between 0.01 and edgelength/2 - 0.01
        // Scale factor is clamped to lie between 0.01 and 0.99 (inclusive)
     void trisectEdge(DLFLEdgePtr edgeptr, double num, bool scale, bool set_type);

        // Subdivide all edges in the object
     void subDivideAllEdges(int num_divs=2, bool set_type=false);

        // Subdivide all edges in the object which are not self loops
     void subDivideNonLoopingEdges(int num_divs=2, bool set_type=false);

        // Split valence 2 vertices into 2 vertices
     void splitValence2Vertices(double offset=-0.1);

        //-- Face Subdivision --//
     void subDivideFace(DLFLFacePtr faceptr, bool usequads=true);
     void subDivideFace(uint face_index, bool usequads=true);
     void subDivideFaces(DLFLFacePtrArray fparray, bool usequads=true);
     void subDivideFaces(DLFLFacePtrList fplist, bool usequads=true);
     void subDivideAllFaces(bool usequads=true);

        //-- Boundary Walk --//
     static void boundaryWalk(DLFLFacePtr faceptr);
     static void boundaryWalk(const DLFLFace& face);
     void boundaryWalk(uint face_index);

        //-- Vertex Trace --//
     static void vertexTrace(DLFLVertexPtr vertexptr);
     static void vertexTrace(const DLFLVertex& vertex);
     void vertexTrace(uint vertex_index);

        //-- Object Subdivision --//
     void loopSubDivide(void);
     void simplestSubDivide(void); // Shannon
     void vertexCuttingSubDivide(double offset=0.25); // Paul
     void pentagonalSubDivide(double offset=0); // Paul
     void pentagonalSubDivide2(double scale_factor=0.75);
     void honeycombSubDivide(void);
     void root4SubDivide(double a=0.0, double twist=0.0);
     void dooSabinSubDivide(bool check=true);
     void cornerCuttingSubDivide(void);
     void modifiedCornerCuttingSubDivide(double thickness); // Esan
     void catmullClarkSubDivide(void);
     void subDivide(void)
       {
         catmullClarkSubDivide();
       }
     void checkerBoardRemeshing(double thickness=0.33);

     void starSubDivide(double offset = 0.0); // Doug
     void sqrt3SubDivide(void); // Doug
     void fractalSubDivide(double offset = 1.0); // Doug

     void stellateSubDivide(void); // Eric
     void twostellateSubDivide(double offset, double curve); // Eric

     void setNormalVertexType(); // Bei & Cansin
     void setOldVertexType(); // Bei & Cansin
     void setNewSubVertexType(); // Bei & Cansin
     void dooSabinSubDivideBC(bool check=true); // Bei & Cansin
     void dooSabinSubDivideBCNew(double sf, double length); // Bei & Cansin
     void loopStyleSubDivide(double length); // Bei & Cansin
     void dual1264SubDivide(double sf); // Bei & Cansin
     void domeSubDivide(double length, double sf);// Bei & Cansin


        //-- Dual --//
        // Flag indicates the "accurate" but slower method should be used.
        // By default we use the faster method which can cause problems in some cases
     void createDual(bool accurate=false);

        //-- Crust Modeling --//
     void createCrust(double thickness, bool uniform=true);
     void createCrustWithScaling(double scale_factor=0.9);
     void cmMakeHole(DLFLFacePtr fp, bool cleanup=true);

        // Tag all faces which match the given face in terms of size
        // Only the outer crust is traversed
        // Face types are set to FTHole
     void tagMatchingFaces(DLFLFacePtr fp);

        //-- Wireframe Modeling (Esan) --//
     void createCrustForWireframe(double thickness=0.1);
     void punchHoles(void);
     void makeWireframe(double crust_thickness=0.1, bool split=true);

        //-- Column Modeling (Esan) --//
     void createWireframeWithSegments(double thickness=0.1,int numSides=4);
     void makeWireframeWithColumns(double wireframe_thickness, int wireframe_segments);

        //-- Output --//
     void objWrite(ostream& o, bool with_normals = false, bool with_tex_coords = false);
     void writeDLFL(ostream& o, bool reverse_faces=false);
  void objPatchWrite( ostream& o );

        //-- Geometric Transformations --//

        // Freeze any stored transformations and reset the transformations
        // Order of application : rotation, scale, translate
     void freezeTransformations(void)
       {
         tr.reset();
         tr.scale(scale_factor);
         tr.rotate(rotation);
         tr.translate(position);
         Matrix4x4 tmat = tr.matrix();
         DLFLVertexPtrList :: iterator vfirst = vertex_list.begin(), vlast = vertex_list.end();
         DLFLVertexPtr vp;
         while ( vfirst != vlast )
            {
              vp = (*vfirst); ++vfirst;
              vp->transform(tmat);
            }
       }

        //-- Rendering --//

        // Apply GL transformations before rendering
     void transform(void) const
       {
         tr.reset();
         tr.scale(scale_factor);
         tr.rotate(rotation);
         tr.translate(position);
         tr.apply();
       }

        // Apply the transformations for this object on the given vector
     void transform(Vector3d& p)
       {
         tr.reset();
         tr.scale(scale_factor);
         tr.rotate(rotation);
         tr.translate(position);
         Vector4d tp(p); tp[3] = 1.0;
         tp = tr.matrix() * tp;
         tp /= tp[3];
         p = tp;
       }

        // Compute normals for all corners using smooth shading
     void computeNormals(void);

        // Do lighting computations for every corner of every face
     void computeLighting(LightPtr lightptr);

        // Render the vertices for selection
     void renderVerticesForSelect(void) const;

        // Render the edges for selection
     void renderEdgesForSelect(void) const;

        // Render the faces for selection
     void renderFacesForSelect(void) const;

        // Render the face vertexes of a given face for selection
     void renderFaceVerticesForSelect(DLFLFacePtr fp) const;

        // Render the object
     void render(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_render);
         glPopMatrix();
       }

     void outline(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_outline);
         glPopMatrix();
       }

     void renderVertices(float pointsize=5.0) const
       {
            // Just render all the vertices with specified point size
         glPushMatrix();
         transform();
         glPointSize(pointsize);
         glBegin(GL_POINTS);
         for_each(vertex_list.begin(),vertex_list.end(),glVertexP);
         glEnd();
         glPointSize(1.0);
         glPopMatrix();
       }

     void renderEdges(float linewidth=3.0) const
       {
            // Just render all the edges with specified line width
         glPushMatrix();
         transform();
         glLineWidth(linewidth);
         for_each(edge_list.begin(),edge_list.end(),renderP);
         glLineWidth(1.0);
         glPopMatrix();
       }

     void renderFaces(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_renderFaces);
         glPopMatrix();
       }

     void outlineFaces(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_outlineFaces);
         glPopMatrix();
       }

     void plainRender(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_plainRender);
         glPopMatrix();
       }

     void plainOutline(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_plainOutline);
         glPopMatrix();
       }

     void renderT(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_renderT);
         glPopMatrix();
       }

     void renderFacesT(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_renderFacesT);
         glPopMatrix();
       }

     void plainRenderT(void) const
       {
         glPushMatrix();
         transform();
         for_each(matl_list.begin(),matl_list.end(),matl_plainRenderT);
         glPopMatrix();
       }

        // Find a material with the given color
        // If no material is found, default material is returned
     DLFLMaterialPtr findMaterial(const RGBColor& color)
       {
         DLFLMaterialPtr matl = NULL;
         DLFLMaterialPtrList :: iterator first, last;
         first = matl_list.begin(); last = matl_list.end();
         while ( first != last )
            {
              if ( (*first)->equals(color) )
                 {
                   matl = (*first); break;
                 }
              ++first;
            }

         return matl;
       }

        // Create a DLFL object from an input stream which should contain
        // an object in OBJ format
     void readObject(istream& i);

        // Alternative version which automatically fills in cracks
     void readObjectAlt(istream& i);

        // Read the object from an input stream in DLFL format
     void readDLFL(istream& i, bool clearold=true);

        // Compute the bounding box for this object
     void boundingBox(Vector3d& min, Vector3d& max) const;

        /*
        ** Static methods to create standard objects
        ** These return a pointer to a newly created DLFLObject
        ** Caller has to free the memory
        */

        // Create a unit cube centered at origin
     static DLFLObjectPtr makeUnitCube(void);

        // Create a cube of specified edge length centered at origin
     static DLFLObjectPtr makeUnitCube(double edgelength);

        // Create a unit tetrahedron centered at origin
     static DLFLObjectPtr makeUnitTetrahedron(void);

        // Create a level 1 Menger sponge using unit cubes
     static DLFLObjectPtr makeMengerSponge(void);

        // Create a level 1 Menger sponge using cubes of specified length
     static DLFLObjectPtr makeMengerSponge(double edgelength);

        // Create a specified level Menger sponge using cubes of unit length
        // This is a recursive function
     static DLFLObjectPtr makeMengerSponge(int level);

        // Create a Sierpinski tetrahedron of specified level
     static DLFLObjectPtr makeSierpinskiTetrahedron(int level);


	/* Patch rendering functions */

  protected :
	// Build the list of patch faces
     void createPatches()
     {
       destroyPatches();
       DLFLFacePtrList :: iterator ffirst = face_list.begin(), flast = face_list.end();
       DLFLFacePtr fp = NULL;
       TMPatchFacePtr pfp = NULL;
       
       while ( ffirst != flast )
	  {
	    fp = (*ffirst); ++ffirst;
	    pfp = new TMPatchFace(patchsize);
	    pfp->setDLFLFace(fp); pfp->createPatches();
	    patch_list.push_back(pfp);
	  }

          // Adjust the edge points for all patches
       DLFLEdgePtrList :: iterator efirst = edge_list.begin(), elast = edge_list.end();
       DLFLEdgePtr ep = NULL;
       DLFLFaceVertexPtr fvp1,fvp2;
       TMPatchPtr pp1, pp2;
       Vector3d p00,p01,p10,p11,ip;
       while ( efirst != elast )
          {
            ep = (*efirst); ++efirst;
            ep->getCorners(fvp1,fvp2);
            pp1 = fvp1->getPatchPtr(); pp2 = fvp2->getPatchPtr();

            p00 = pp1->getControlPoint(2,0); p01 = pp2->getControlPoint(2,0);
            p10 = pp1->getControlPoint(3,1); p11 = pp2->getControlPoint(3,1);
            ip = intersectCoplanarLines(p00,p01,p10,p11);

            pp1->setControlPoint(3,0,ip); pp2->setControlPoint(3,0,ip);
            pp1->updateGLPointArray(); pp2->updateGLPointArray();

            pp1 = fvp1->next()->getPatchPtr(); pp2 = fvp2->next()->getPatchPtr();
            pp1->setControlPoint(0,3,ip); pp2->setControlPoint(0,3,ip);
            pp1->updateGLPointArray(); pp2->updateGLPointArray();
          }

          // Adjust the vertex points for 4-valence vertices
       DLFLVertexPtrList :: iterator vfirst = vertex_list.begin(), vlast = vertex_list.end();
       DLFLVertexPtr vp = NULL;
       while ( vfirst != vlast )
          {
            vp = (*vfirst); ++vfirst;
            if ( vp->valence() == 4 )
               {
                 DLFLFaceVertexPtrArray vcorners;
                 vp->getOrderedCorners(vcorners);
                 pp1 = vcorners[0]->getPatchPtr(); pp2 = vcorners[2]->getPatchPtr();

                 p00 = pp1->getControlPoint(1,0); p01 = pp2->getControlPoint(1,0);
                 p10 = pp1->getControlPoint(0,1); p11 = pp2->getControlPoint(0,1);
                 ip = intersectCoplanarLines(p00,p01,p10,p11);

                 for (int i=0; i < 4; ++i)
                    {
                      pp1 = vcorners[i]->getPatchPtr();
                      pp1->setControlPoint(0,0,ip);
                      pp1->updateGLPointArray();
                    }
               }
          }
                 
          /*
       TMPatchFacePtrList :: iterator pfirst = patch_list.begin(), plast = patch_list.end();
       while ( pfirst != plast )
	  {
	    pfp = (*pfirst); ++pfirst;
            pfp->adjustEdgePoints();
	  }
          */
     }

  public :

     void updateForPatches(void)
     {
	  // Update information stored at each face, vertex, edge and corner for patch rendering

	  // Compute doo-sabin coordinates for each face and store them in the auxcoord field of the corner
	  // Update the auxcoord field of the face
       Vector3dArray coords;
       DLFLFacePtrList :: iterator ffirst=face_list.begin(), flast=face_list.end();
       DLFLFacePtr fp;
       DLFLFaceVertexPtrArray corners;
       int valence;
       
       while ( ffirst != flast )
	  {
	    fp = *ffirst; ++ffirst;
	    fp->getCornersAndCoords(corners,coords);
	    valence = coords.size();

            if ( valence > 0 )
               {
                    // Compute Doo-Sabin coordinates - Level 1
                 computeDooSabinCoords(coords);
                 for (int i=0; i < valence; ++i) corners[i]->setAuxCoords(coords[i]);

                    // Compute Doo-Sabin coordinates - Level 2
                 computeDooSabinCoords(coords);
                 for (int i=0; i < valence; ++i) corners[i]->setDS2Coord2(coords[i]);

                    // Compute the patch point and patch normal
                 Vector3d pp, pn;
                 computeCentroidAndNormal(coords,pp,pn);
                 fp->setAuxCoords(pp); fp->setAuxNormal(pn);
               }
	  }

          // Compute patch point and normal for all edges
       DLFLEdgePtrList :: iterator efirst=edge_list.begin(), elast=edge_list.end();
       DLFLEdgePtr ep;
       while ( efirst != elast )
	  {
	    ep = *efirst; ++efirst;

            Vector3dArray p;
            ep->getEFCornersAuxCoords(p);
            
               // Compute Doo-Sabin coordinates - Level 2
            computeDooSabinCoords(p);

            Vector3d pp,pn;
            computeCentroidAndNormal(p,pp,pn);
	    ep->setAuxCoords(pp); ep->setAuxNormal(pn);

            DLFLFaceVertexPtrArray fvp;
            ep->getEFCorners(fvp);
            fvp[0]->setDS2Coord3(p[0]); fvp[1]->setDS2Coord1(p[1]);
            fvp[2]->setDS2Coord3(p[2]); fvp[3]->setDS2Coord1(p[3]);
          }

          // Compute patch point and normal for all vertices
       DLFLVertexPtrList :: iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
       DLFLVertexPtr vp;
       while ( vfirst != vlast )
          {
            vp = *vfirst; ++vfirst;
            Vector3dArray p;
            vp->getOrderedCornerAuxCoords(p);

               // Compute Doo-Sabin coordinates - Level 2
            computeDooSabinCoords(p);

            Vector3d pp,pn;
            computeCentroidAndNormal(p,pp,pn);
            vp->setAuxCoords(pp); vp->setAuxNormal(-pn); // Reverse the normal since the rotation order around the vertex is clockwise
            
            DLFLFaceVertexPtrArray fvp;
            vp->getOrderedCorners(fvp);
            for (int i=0; i < fvp.size(); ++i) fvp[i]->setDS2Coord0(p[i]);
          }
     }

	// Set the patch size
     void setPatchSize(int size)
     {
       if ( size != patchsize && size > 0 )
	  {
	    patchsize = size;
	    createPatches();
	  }
     }

	// Update the patches
     void updatePatches()
     {
       updateForPatches();
       createPatches();
     }

        // Render the patches
     void renderPatches(void) const
       {
         glPushMatrix();
         transform();
         for_each(patch_list.begin(), patch_list.end(), tmpf_render);
         glPopMatrix();
       }

        // Render the object using wireframe patches
     void renderWireframePatches(void) const
       {
         glPushMatrix();
         transform();
         for_each(patch_list.begin(), patch_list.end(), tmpf_outline);
         glPopMatrix();
       }

     void renderPatchBoundaries(void) const
       {
         glPushMatrix();
         transform();
         for_each(patch_list.begin(), patch_list.end(), tmpf_patch_boundary);
         glPopMatrix();
       }
         
     void renderPatchFaceBoundaries(void) const
       {
         glPushMatrix();
         transform();
         for_each(patch_list.begin(), patch_list.end(), tmpf_face_boundary);
         glPopMatrix();
       }

     void renderDS2Points(void) const
       {
         Vector3d ds20,ds21,ds22,ds23;
         DLFLFacePtrList :: const_iterator ffirst=face_list.begin(), flast=face_list.end();
         DLFLFacePtr fp;
         DLFLFaceVertexPtrArray corners;

         glBegin(GL_POINTS);
         while ( ffirst != flast )
            {
              fp = *ffirst; ++ffirst;
              fp->getCorners(corners);

              for (int i=0; i < corners.size(); ++i)
                 {
                   corners[i]->getDS2Coords(ds20,ds21,ds22,ds23);

                   glColor3f(1.0,0.0,0.0);
                   glVertex(corners[i]->getAuxCoords());

                   glColor3f(0.0,1.0,0.0);
                   glVertex(ds20);
                   glVertex(ds21);
                   glVertex(ds22);
                   glVertex(ds23);
                 }
            }
         glVertex(fp->getAuxCoords());
         glEnd();
       }
     
        // Render the object using point patches
     void renderPointPatches(void) const
       {
         glPushMatrix();
         transform();
         for_each(patch_list.begin(), patch_list.end(), tmpf_controlpoints);
//         renderDS2Points();
         glPopMatrix();
       }

     
     void renderPatchNormals(void) const
       {
         glPushMatrix();
         transform();
         for_each(patch_list.begin(), patch_list.end(), tmpf_render_normals);
         glPopMatrix();
       }
};

#endif /* #ifndef _DLFL_OBJECT_HH_ */


/*
  $Log: DLFLObject.hh,v $
  Revision 1.1.1.1  2007/03/10 18:32:17  stuart
  Initial CVS Import

  Revision 4.9  2004/01/20 22:17:52  vinod
  Added Loop subdivision scheme

  Revision 4.8  2004/01/20 09:26:12  vinod
  Added variables and methods for column modeling

  Revision 4.7  2004/01/20 07:23:28  vinod
  Added checkerBoardRemeshing scheme. Changed name of subhw to dual1264

  Revision 4.6  2004/01/20 00:55:38  vinod
  Changed name of Bei and Cansin's Loop remeshing scheme

  Revision 4.5  2004/01/19 20:19:54  vinod
  Added remeshing schemes by Bei & Cansin

  Revision 4.4  2004/01/17 00:52:41  vinod
  Added tagMatchingFaces method

  Revision 4.3  2004/01/13 02:46:00  vinod
  Changed names of dodecahedral and icosahedral extrude methods

  Revision 4.2  2004/01/13 02:12:31  vinod
  Added double stellation, dodecahedral and icosahedral extrusion,
  stellate and double stellate remeshing schemes.

  Revision 4.1  2004/01/02 21:00:47  vinod
  Additions by Doug: star, sqrt3 and fractal subdivision

  Revision 4.0  2003/12/26 01:58:53  vinod
  Major version sync.

  Revision 3.16  2003/12/08 20:18:35  vinod
  Added tetrahedron and Sierpinski tetrahedron to standard objects

  Revision 3.15  2003/12/08 17:02:35  vinod
  Added glBegin and glEnd in renderVertices method. Added methods for creating standard objects such as unit cube and Menger sponge

  Revision 3.14  2003/12/07 01:48:39  vinod
  Added flag to createDual to allow choice of methods

  Revision 3.13  2003/12/01 09:00:29  vinod
  Added freezeTransformations method

  Revision 3.12  2003/11/29 22:03:51  vinod
  Added max. connections parameter for regular connectFaces

  Revision 3.11  2003/11/29 20:37:17  vinod
  Added subDivideFaces method with linked list parameter.
  Added subDivideAllFaces method

  Revision 3.10  2003/11/29 19:11:54  vinod
  Added cleanupWingedVertices and subDivideFaces methods

  Revision 3.9  2003/11/27 00:22:43  vinod
  Changed threshold angle to threshold factor in Menger sponge creation

  Revision 3.8  2003/11/26 18:25:26  vinod
  Added genus() method.
  Changed collapse threshold parameter in Menger sponge creation to angle instead of length

  Revision 3.7  2003/11/23 22:43:58  vinod
  Removed fractional thickness parameter and
  added collapse threshold parameter for createSponge

  Revision 3.6  2003/11/21 15:25:00  vinod
  Added fractional thickness flag for menger sponge creation.
  Added cleanup2gons method

  Revision 3.5  2003/11/16 06:44:49  vinod
  Added duplicateFacePlanarOffset and extrudeFacePlanarOffset methods

  Revision 3.4  2003/10/21 06:46:10  vinod
  Added make????Unique methods. Added createSponge.
  Modified trisectEdge to handle offset distancese also

  Revision 3.3  2003/10/17 00:59:08  vinod
  Modified crust creation to allow creating uniform thickness crusts

  Revision 3.2  2003/09/26 17:12:27  vinod
  Added modifiedMultiConnectCrust

  Revision 3.1  2003/09/26 15:30:55  vinod
  Added duplicateFace, multiConnectCrust, trisectEdge methods

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.46  2003/09/19 16:03:45  vinod
  Added subDivideNonLoopingEdges method

  Revision 2.45  2003/09/10 15:16:20  vinod
  Added multiConnectMidpoints

  Revision 2.44  2003/08/24 23:11:13  vinod
  Added set_type parameter to subDivideAllEdges. Added pentagonalSubDivide2

  Revision 2.43  2003/08/24 20:27:21  vinod
  Added new multiConnect method using iterative search

  Revision 2.42  2003/07/07 16:11:22  vinod
  Added findMaxOffsets, use_max_offsets option for multiConnectFaces

  Revision 2.41  2003/04/30 18:05:16  vinod
  Moved reverse method to source file

  Revision 2.40  2003/04/25 15:44:19  vinod
  Added splice, reverse methods.
  Added get* methods for access to internal lists through arrays

  Revision 2.39  2003/04/18 21:30:07  vinod
  Added multiConnectFaces using convex hull

  Revision 2.38  2003/04/04 06:28:14  vinod
  Added edgeCleanup method

  Revision 2.37  2003/03/27 08:17:36  vinod
  Changed original multiConnect to tripleConnect.
  Added new multiConnectFaces which takes a DLFLFacePtrArray

  Revision 2.36  2003/03/12 07:36:38  vinod
  Added 2-gon cleanup option to collapseEdge

  Revision 2.35  2003/03/10 07:34:13  vinod
  Added collapseEdge

  Revision 2.34  2003/01/30 07:02:45  vinod
  Added multiConnectFaces

  Revision 2.33  2003/01/25 19:43:18  vinod
  Indentation change

  Revision 2.32  2003/01/25 08:13:16  vinod
  Changed default offset to be negative in splitValence2Vertices.
  Added boolean split flag to makeWireframe method

  Revision 2.31  2003/01/20 21:43:35  vinod
  Name change from skeleton modeling to wireframe modeling

  Revision 2.30  2003/01/05 21:11:58  vinod
  Removed unused variable

  Revision 2.29  2002/12/30 17:28:15  vinod
  Added splitValence2Vertices

  Revision 2.28  2002/12/30 04:07:53  vinod
  Added methods related to Skeleton Modeling

  Revision 2.27  2002/12/18 16:22:37  vinod
  Added face subdivision and tile texturing methods

  Revision 2.26  2002/12/17 03:53:40  vinod
  Fixed comment logs

  Revision 2.25  2002/12/17 03:18:37  vinod
  See comments for previous revision

  Revision 2.24  2002/12/17 02:57:38  vinod
  Additions by Paul
  dualConnectFaces for triangularly connecting faces
  extrudeDualFace, stellateFace
  Vertex cutting and pentagonal subdivision schemes

  Revision 2.23  2002/12/17 02:44:59  vinod
  DLFLWindow.hh

  Revision 2.22  2002/11/24 00:52:02  vinod
  Added connectEdges version without loopcheck

  Revision 2.21  2002/11/07 08:19:21  vinod
  Added edge check flag for dooSabin subdivision

  Revision 2.20  2002/10/31 06:07:00  vinod
  Added simplest subdivision scheme

  Revision 2.19  2002/10/22 21:23:06  vinod
  Added createDual method

  Revision 2.18  2002/04/28 22:15:06  vinod
  Added variant of edgeExists to check two DLFLVertexes

  Revision 2.17  2002/04/23 21:34:46  vinod
  Added alternate insertEdge and readObject functions

  Revision 2.16  2001/12/22 23:10:27  vinod
  Added weight factor for root-4 subdivision

  Revision 2.15  2001/12/17 17:35:27  vinod
  Added extrudeFaceDS - dooSabin type extrude

  Revision 2.14  2001/12/12 18:21:32  vinod
  Added option of choosing closest corners for creating hole/handle.
  Added option for multiple extrusions of the same face

  Revision 2.13  2001/12/05 18:14:27  vinod
  Added crust modeling using scaling, cleanup option
  Min. face id is now stored instead of calculated at hole creation to allow cleanup

  Revision 2.12  2001/11/21 00:18:20  vinod
  Added planarize, spheralize, meshsmooth, crust modeling, flags in read/writeDLFL

  Revision 2.11  2001/11/11 21:55:50  vinod
  Added subDivideAllEdges

  Revision 2.10  2001/11/02 00:34:48  vinod
  Added option for extra twists and multiple edge subdivisions

  Revision 2.9  2001/10/22 22:48:58  vinod
  Added cleanup option to deleteEdge.
  Changed order of parameters with default values in createFace

  Revision 2.8  2001/10/06 19:01:23  vinod
  Added separate option for corner-cutting subdivision

  Revision 2.7  2001/10/06 18:36:34  vinod
  Added check flag to connectEdges. Added insertEdgeWithoutCheck

  Revision 2.6  2001/10/06 16:40:32  vinod
  Added catmullClarkSubDivide method, made default

  Revision 2.5  2001/09/28 17:45:38  vinod
  createPointSphere now returns the FaceVertex

  Revision 2.4  2001/09/28 05:51:15  vinod
  Removed extra call to add face to material
  Added dooSabinSubDivide and connectEdges methods

  Revision 2.3  2001/09/19 03:57:43  vinod
  Added rotation and scaling for extrude

  Revision 2.2  2001/09/15 22:39:38  vinod
  Changed member variable name from scale to scale_factor

  Revision 2.1  2001/08/07 07:15:18  vinod
  subDivideEdge now returns the new Vertex

  Revision 2.0  2001/07/25 05:12:45  vinod
  Major version sync

  Revision 1.14  2001/07/18 07:13:17  vinod
  Modified addFacePtr to use faceptr in material->addFace

  Revision 1.13  2001/07/16 04:37:41  vinod
  Added createPointSphere

  Revision 1.11  2001/07/11 06:06:50  vinod
  insertEdge functions now return the EdgePtr. Added randomAssignTexCoords

  Revision 1.10  2001/05/17 05:00:00  vinod
  Added numsegs and weights as parameters for bezier and hermite connect fns

  Revision 1.9  2001/05/15 19:37:57  vinod
  Added createFace

  Revision 1.8  2001/04/25 20:02:20  vinod
  Added extrudeFace(DLFLFacePtr), bezier/hermiteConnectFaces

  Revision 1.7  2001/04/08 21:19:45  vinod
  Added findVertex/Edge/Face, extrudeFace, read/writeDLFL

  Revision 1.6  2000/12/13 22:22:32  vinod
  Added tex coordinate output, deleteVertex

  Revision 1.5  2000/11/28 19:15:06  vinod
  Added computeNormals and computeLighting

  Revision 1.4  2000/11/22 07:41:53  vinod
  Added connectFaces subroutine

  Revision 1.3  2000/11/21 19:20:34  vinod
  Added dump() method. Split insertEdge into 2 methods

  Revision 1.2  2000/11/02 22:17:53  vinod
  Update insertEdge to set material for the new faces correctly

  Revision 1.1.1.1  2000/11/02 19:32:43  vinod
  New DLFL Sources

*/
