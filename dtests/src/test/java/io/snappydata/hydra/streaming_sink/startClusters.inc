INCLUDE $JTESTS/hydraconfig/hydraparams1.inc;
INCLUDE $JTESTS/hydraconfig/topology_3.inc;

hydra.GemFirePrms-names = gemfire1;
hydra.ClientPrms-gemfireNames = gemfire1;
hydra.GemFirePrms-distributedSystem = ds;
io.snappydata.hydra.cluster.SnappyPrms-kafkaDir="${kafkaDir}";
//io.snappydata.hydra.cluster.SnappyPrms-snappyPocJarPath="${snappyPocJarPath}";

THREADGROUP snappyStoreThreads
            totalThreads = fcn "(${${A}Hosts} * ${${A}VMsPerHost} *  ${${A}ThreadsPerVM}) -1 " ncf
            totalVMs     = fcn "(${${A}Hosts} * ${${A}VMsPerHost})" ncf
            clientNames  = fcn "hydra.TestConfigFcns.generateNames(\"${A}\",
                                ${${A}Hosts}, true)" ncf;

THREADGROUP leadThreads
            totalThreads = fcn "(${${B}Hosts} * ${${B}VMsPerHost} *  ${${B}ThreadsPerVM}) -1 " ncf
            totalVMs     = fcn "(${${B}Hosts} * ${${B}VMsPerHost})" ncf
            clientNames  = fcn "hydra.TestConfigFcns.generateNames(\"${B}\",
                                ${${B}Hosts}, true)" ncf;

THREADGROUP locatorThreads
            totalThreads = fcn "(${${C}Hosts} * ${${C}VMsPerHost} *  ${${C}ThreadsPerVM}) " ncf
            totalVMs     = fcn "(${${C}Hosts} * ${${C}VMsPerHost})" ncf
            clientNames  = fcn "hydra.TestConfigFcns.generateNames(\"${C}\",
                                ${${C}Hosts}, true)" ncf;

THREADGROUP snappyThreads
            totalThreads = 1
            totalVMs     = 1
            clientNames  = fcn "hydra.TestConfigFcns.generateNames(\"${A}\",${${A}Hosts}, true)"  ncf;

THREADGROUP leadThread
            totalThreads = 1
            totalVMs     = 1
            clientNames  = fcn "hydra.TestConfigFcns.generateNames(\"${B}\",${${B}Hosts}, true)" ncf;

INITTASK     taskClass   = io.snappydata.hydra.cluster.SnappyTest taskMethod  = HydraTask_initializeSnappyTest
    runMode = always
    threadGroups = snappyThreads, locatorThreads, snappyStoreThreads, leadThreads,leadThread;

INITTASK    taskClass   = util.StopStartVMs  taskMethod = StopStart_initTask
    threadGroups = snappyThreads, locatorThreads, snappyStoreThreads, leadThreads,leadThread;

INITTASK     taskClass   = io.snappydata.hydra.adAnalytics.SnappyAdAnalyticsTest taskMethod  = HydraTask_initializeSnappyAdAnalyticsTest
    runMode = always
    threadGroups = snappyThreads,leadThread;

INITTASK     taskClass   = io.snappydata.hydra.cluster.SnappyTest taskMethod  = HydraTask_generateSnappyLocatorConfig
    runMode = always
    threadGroups = locatorThreads;

INITTASK     taskClass   = io.snappydata.hydra.cluster.SnappyTest taskMethod  = HydraTask_writeLocatorConfigData
    runMode = always
    threadGroups = snappyThreads;

INITTASK     taskClass   = io.snappydata.hydra.cluster.SnappyTest taskMethod  = HydraTask_generateSnappyServerConfig
    runMode = always
    threadGroups = snappyStoreThreads,snappyThreads;

INITTASK     taskClass   = io.snappydata.hydra.cluster.SnappyTest taskMethod  = HydraTask_writeServerConfigData
    runMode = always
    threadGroups = snappyThreads;

INITTASK     taskClass   = io.snappydata.hydra.cluster.SnappyTest taskMethod  = HydraTask_generateSnappyLeadConfig
    runMode = always
    threadGroups = leadThreads, leadThread;

INITTASK     taskClass   = io.snappydata.hydra.cluster.SnappyTest taskMethod  = HydraTask_writeLeadConfigData
    runMode = always
    threadGroups = snappyThreads;

INITTASK     taskClass   = io.snappydata.hydra.adAnalytics.SnappyAdAnalyticsTest taskMethod  = HydraTask_StartKafkaZookeeper
    runMode = always
    threadGroups = snappyThreads;

INITTASK     taskClass   = io.snappydata.hydra.adAnalytics.SnappyAdAnalyticsTest taskMethod  = HydraTask_StartKafkaBrokers
    runMode = always
    threadGroups = snappyThreads;

INITTASK    taskClass   = io.snappydata.hydra.cluster.SnappyTest taskMethod  = HydraTask_startSnappyCluster
    runMode = always
    threadGroups = snappyThreads;

INITTASK     taskClass   = io.snappydata.hydra.testDMLOps.SnappyDMLOpsUtil taskMethod  = HydraTask_initialize
	threadGroups = snappyThreads;


INITTASK     taskClass   = io.snappydata.hydra.adAnalytics.SnappyAdAnalyticsTest taskMethod  = HydraTask_executeSnappyJob
    io.snappydata.hydra.cluster.SnappyPrms-jobClassNames = io.snappydata.hydra.streaming_sink.CreateTableInSnappyJob
    io.snappydata.hydra.cluster.SnappyPrms-appPropsForJobServer = "isRowTable=${isRowTable},withKeyColumn=${withKeyColumn},isPartitioned=${isPartitioned}"
    io.snappydata.hydra.cluster.SnappyPrms-userAppJar = snappydata-store-scala-tests*tests.jar
    threadGroups = snappyThreads;

io.snappydata.hydra.testDMLOps.SnappySchemaPrms-largeDataSet = true;

INCLUDE $JTESTS/io/snappydata/hydra/testDMLOps/startDerby.inc;
INCLUDE $JTESTS/io/snappydata/hydra/streaming_sink/schema.inc;
