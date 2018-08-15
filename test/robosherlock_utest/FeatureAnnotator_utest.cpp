#include <gtest/gtest.h>
#include <rs/scene_cas.h>
#include <rs/flowcontrol/RSAnalysisEngine.h>
#undef OUT_LEVEL
#define OUT_LEVEL OUT_LEVEL_DEBUG
#include "../main.h"

void processFeatureCluster(uima::CAS *cas)
{
  rs::SceneCas sceneCas(*cas);
  if (cas == NULL) outError("The CAS is null");
  rs::Scene scene = sceneCas.getScene();
  std::vector<rs::Cluster> clusters;
  try{
  scene.identifiables.filter(clusters);
  }
  catch(const char* e)
  {
    outError("Could not obtain clusters");
    outError(e);
  }
  EXPECT_TRUE(clusters.size()>0);
  for (int i = 0; i<clusters.size();i++)
  {
    rs::Cluster &cluster = clusters[i];
    std::vector<rs::Features> features;
   
    cluster.annotations.filter(features);
    for ( int j = 0; j<features.size();j++)
    {
      outError(features[j].descriptorType.get());
      EXPECT_TRUE(features[j].descriptorType.get() != "NULL");
    }
  }
}
void featureAnnotator()
{

  std::vector<std::string> engineList = {"CollectionReader","ImagePreprocessor","NormalEstimator","PlaneAnnotator","FeatureAnnotator"};
  engine.getPipelineManager()->setPipelineOrdering(engineList);
  
  engine.process();
  cas = engine.getCas();
  processFeatureCluster(cas);
  
  
  engine.overwriteParam("FeatureAnnotator","keypointDetector",std::string("BRISK"));
  engine.overwriteParam("FeatureAnnotator","featureExtractor",std::string("BRISK")); 
  engine.reconfigure();
  engine.process();
  cas = engine.getCas();
  processFeatureCluster(cas);
  
  engine.overwriteParam("FeatureAnnotator","keypointDetector",std::string("ORB"));
  engine.overwriteParam("FeatureAnnotator","featureExtractor",std::string("ORB")); 
  engine.reconfigure();
  engine.process();
  cas = engine.getCas();
  processFeatureCluster(cas);
  
  /*engine.overwriteParam("FeatureAnnotator","keypointDetector",std::string("FAST"));
  engine.overwriteParam("FeatureAnnotator","featureExtractor",std::string("FREAK")); 
  engine.reconfigure();
  engine.process();
  cas = engine.getCas();
  processFeatureCluster(cas);*/		
  outError("DONE");
  
}

TEST(UnitTest,FeatureAnnotator)
{
  featureAnnotator();
}