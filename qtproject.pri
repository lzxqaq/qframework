!isEmpty(QTPROJECT_PRI_INCLUDED):error("qtproject.pri already included")
QTPROJECT_PRI_INCLUDED = 1

isEmpty(CONFIG_PRI_INCLUDED){include(config.pri)}
