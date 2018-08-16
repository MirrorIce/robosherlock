#include <gtest/gtest.h>
#include <rs/scene_cas.h>
#include <rs/flowcontrol/RSAnalysisEngine.h>
#undef OUT_LEVEL
#define OUT_LEVEL OUT_LEVEL_DEBUG
#include "../main.h"


void cluster3DGeometryTest()
{

  std::vector<std::string> engineList = {"CollectionReader","ImagePreprocessor","NormalEstimator","PlaneAnnotator","Cluster3DGeometryAnnotator"};
  engine.getPipelineManager()->setPipelineOrdering(engineList);
  
  engine.process();
  cas = engine.getCas();
  
  if (cas == NULL) outError("The CAS is null");
  rs::SceneCas sceneCas(*cas);
  
  rs::Scene scene = sceneCas.getScene();
  std::vector<rs::Cluster> clusters;
  scene.identifiables.filter(clusters);
  EXPECT_TRUE(clusters.size()>0);
  
  for (int i = 0; i<clusters.size();i++)
  {
    rs::Cluster &cluster = clusters[i];
    
    std::vector<rs::Geometry> geometry;
   
    cluster.annotations.filter(geometry);
    for (int i = 0; i<geometry.size();i++)
    {
      rs::BoundingBox3D boundingBox = geometry[i].boundingBox.get();
      EXPECT_TRUE(boundingBox.width.get()>0);
      EXPECT_TRUE(boundingBox.height.get()>0);
      EXPECT_TRUE(boundingBox.depth.get()>0);
      EXPECT_TRUE(boundingBox.volume.get()>0);
      //Checks if the calculated boundingBox width height depth volume corresponds to the written volume
      EXPECT_TRUE( abs( boundingBox.width.get()*boundingBox.height.get()*boundingBox.depth.get() - boundingBox.volume.get() ) - 0.00001 < 0     );   
    }
  }
  
  
  
}

TEST(UnitTest,Cluster3DGeometry)
{
  cluster3DGeometryTest();
}
