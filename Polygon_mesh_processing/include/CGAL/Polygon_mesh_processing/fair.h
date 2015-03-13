#ifndef CGAL_POLYGON_MESH_PROCESSING_FAIR_H
#define CGAL_POLYGON_MESH_PROCESSING_FAIR_H

#include <CGAL/internal/Meshing_functions/Fair_Polyhedron_3.h>

namespace CGAL {

namespace Polygon_mesh_processing {

namespace internal {
  // use non-default weight calculator and non-default solver
  // WeightCalculator a model of `FairWeightCalculator`, can be omitted to use default Cotangent weights
  // weight_calculator a function object to calculate weights, defaults to Cotangent weights and can be omitted
  template<class SparseLinearSolver, class WeightCalculator, class PolygonMesh, class VertexRange>
  bool fair(PolygonMesh& pmesh,
    VertexRange vertices,
    SparseLinearSolver solver,
    WeightCalculator weight_calculator,
    Fairing_continuity continuity = FAIRING_C_1)
  {
    CGAL::Polygon_mesh_processing::internal::Fair_Polyhedron_3<PolygonMesh,
      SparseLinearSolver, WeightCalculator> fair_functor(pmesh, weight_calculator);
    return fair_functor.fair(vertices, solver, continuity);
  }

  //use default SparseLinearSolver
  template<class WeightCalculator, class PolygonMesh, class VertexRange>
  bool fair(PolygonMesh& pmesh,
    VertexRange vertices,
    CGAL::Default,
    WeightCalculator weight_calculator,
    Fairing_continuity continuity = FAIRING_C_1)
  {
    typedef typename internal::Fair_default_sparse_linear_solver::Solver Sparse_linear_solver;
    return internal::fair(pmesh, vertices, Sparse_linear_solver(), weight_calculator, continuity);
  }
} //end namespace internal

  /*!
  \ingroup PkgPolygonMeshProcessing
  @brief Function fairing a region on a polygon mesh.
  The region described by @a vertices might contain multiple disconnected components.
  Note that the structure is not altered in any way, only positions of the vertices get updated.

  Fairing might fail if fixed vertices, which are used as boundary conditions, do not suffice to solve constructed linear system.
  The larger @a continuity gets, the more fixed vertices are required.

  @tparam SparseLinearSolver a model of `SparseLinearAlgebraTraitsWithFactor_d`. If \ref thirdpartyEigen "Eigen" 3.2 (or greater) is available
  and `CGAL_EIGEN3_ENABLED` is defined, then an overload of `Eigen_solver_traits` is provided as default parameter.
  @tparam PolygonMesh a model of `MutableFaceGraph`
  @tparam VertexRange a range of vertices of `PolygonMesh`, model of `SinglePassRange`

  @param pmesh the polygon mesh to be faired
  @param vertices the range of vertices allowed to move
  @param solver an instance of the sparse linear solver to use. It defaults to the 
         default construtor of the `SparseLinearSolver` template parameter.
  @param continuity tangential continuity, defaults to `FAIRING_C_1` and can be omitted

  @return `true` if fairing is successful, otherwise no vertex position is changed

  @todo document solver parameter
  @todo accuracy of solvers are not good, for example when there is no boundary condition pre_factor should fail, but it does not.
  */
  template<class SparseLinearSolver, class PolygonMesh, class VertexRange>
  bool fair(PolygonMesh& pmesh,
    VertexRange vertices,
    SparseLinearSolver solver
#ifdef DOXYGEN_RUNNING
    = CGAL::Default()
#endif
    , Fairing_continuity continuity = FAIRING_C_1)
  {
    typedef CGAL::internal::Cotangent_weight_with_voronoi_area_fairing<PolygonMesh> Weight_calculator;
    return internal::fair<SparseLinearSolver, Weight_calculator, PolygonMesh, VertexRange>
      (pmesh, vertices, solver, Weight_calculator(pmesh), continuity);
  }

  //use default SparseLinearSolver and WeightCalculator
  template<class PolygonMesh, class VertexRange>
  bool fair(PolygonMesh& pmesh,
    VertexRange vertices,
    CGAL::Default,
    Fairing_continuity continuity = FAIRING_C_1)
  {
    typedef internal::Fair_default_sparse_linear_solver::Solver Sparse_linear_solver;
    return fair(pmesh, vertices, Sparse_linear_solver(), continuity);
  }

  //use default SparseLinearSolver and WeightCalculator
  template<class PolygonMesh, class VertexRange>
  bool fair(PolygonMesh& pmesh,
            VertexRange vertices)
  {
    return fair(pmesh, vertices, CGAL::Default());
  }

} //end namespace Polygon_mesh_processing

} //end namespace CGAL

#endif //CGAL_POLYGON_MESH_PROCESSING_FAIR_H
