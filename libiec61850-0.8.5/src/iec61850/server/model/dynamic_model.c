/*
 *  dynamic_model.c
 *
 *  Copyright 2014 Michael Zillgith
 *
 *  This file is part of libIEC61850.
 *
 *  libIEC61850 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  libIEC61850 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with libIEC61850.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  See COPYING file for the complete license text.
 */


#include "iec61850_server.h"
#include "string_utilities.h"
#include "libiec61850_platform_includes.h"
#include "stack_config.h"

static void
iedModel_emptyVariableInitializer(void)
{
    return;
}

IedModel*
IedModel_create(const char* name/*, MemoryAllocator allocator*/)
{
    IedModel* self = (IedModel*) GLOBAL_CALLOC(1, sizeof(IedModel));

    if (name)
        self->name = copyString(name);
    else
        self->name = NULL;

    self->rcbs = NULL;

    self->dataSets = NULL;

    self->gseCBs = NULL;

    self->sgcbs = NULL;

    self->initializer = iedModel_emptyVariableInitializer;

    return self;
}

static void
IedModel_addDataSet(IedModel* self, DataSet* dataSet)
{
    if (self->dataSets == NULL)
        self->dataSets = dataSet;
    else {
        DataSet* lastDataSet = self->dataSets;

        while (lastDataSet != NULL) {
            if (lastDataSet->sibling == NULL) {
                lastDataSet->sibling = dataSet;
                break;
            }

            lastDataSet = lastDataSet->sibling;
        }
    }
}

static void
IedModel_addLogicalDevice(IedModel* self, LogicalDevice* lDevice)
{
    if (self->firstChild == NULL)
        self->firstChild = lDevice;
    else {
        LogicalDevice* sibling = self->firstChild;

        while (sibling->sibling != NULL)
            sibling = (LogicalDevice*) sibling->sibling;

        sibling->sibling = (ModelNode*) lDevice;
    }
}

static void
IedModel_addReportControlBlock(IedModel* self, ReportControlBlock* rcb)
{
    if (self->rcbs == NULL)
        self->rcbs = rcb;
    else {
        ReportControlBlock* lastRcb = self->rcbs;

        while (lastRcb->sibling != NULL)
            lastRcb = lastRcb->sibling;

        lastRcb->sibling = rcb;
    }
}

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
static void
IedModel_addSettingGroupControlBlock(IedModel* self, SettingGroupControlBlock* sgcb)
{
    if (self->sgcbs == NULL)
        self->sgcbs = sgcb;
    else {
        SettingGroupControlBlock* lastSgcb = self->sgcbs;

        while (lastSgcb->sibling != NULL)
            lastSgcb = lastSgcb->sibling;

        lastSgcb->sibling = sgcb;
    }
}
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */


static void
IedModel_addGSEControlBlock(IedModel* self, GSEControlBlock* gcb)
{
    if (self->gseCBs == NULL)
        self->gseCBs = gcb;
    else {
        GSEControlBlock* lastGcb = self->gseCBs;

        while (lastGcb->sibling != NULL)
            lastGcb = lastGcb->sibling;

        lastGcb->sibling = gcb;
    }
}

LogicalDevice*
LogicalDevice_create(const char* name, IedModel* parent)
{
    LogicalDevice* self = (LogicalDevice*) GLOBAL_CALLOC(1, sizeof(LogicalDevice));

    self->name = copyString(name);
    self->modelType = LogicalDeviceModelType;
    self->parent = (ModelNode*) parent;
    self->sibling = NULL;

    IedModel_addLogicalDevice(parent, self);

    return self;
}

static LogicalNode*
LogicalDevice_getLastLogicalNode(LogicalDevice* self)
{
    LogicalNode* lastNode = (LogicalNode*) self->firstChild;

    LogicalNode* nextNode = lastNode;

    while (nextNode != NULL) {
        lastNode = nextNode;
        nextNode = (LogicalNode*) nextNode->sibling;
    }

    return lastNode;
}

static void
LogicalDevice_addLogicalNode(LogicalDevice* self, LogicalNode* lNode)
{
    if (self->firstChild == NULL)
        self->firstChild = (ModelNode*) lNode;
    else {
        LogicalNode* lastNode = LogicalDevice_getLastLogicalNode(self);

        lastNode->sibling = (ModelNode*) lNode;
    }
}

LogicalNode*
LogicalNode_create(const char* name, LogicalDevice* parent)
{
    LogicalNode* self = (LogicalNode*) GLOBAL_MALLOC(sizeof(LogicalNode));

    self->name = copyString(name);
    self->parent = (ModelNode*) parent;
    self->modelType = LogicalNodeModelType;
    self->firstChild = NULL;
    self->sibling = NULL;

    LogicalDevice_addLogicalNode(parent, self);

    return self;
}

static DataObject*
LogicalNode_getLastDataObject(LogicalNode* self)
{
    DataObject* lastNode = (DataObject*) self->firstChild;

    DataObject* nextNode = lastNode;

    while (nextNode != NULL) {
        lastNode = nextNode;
        nextNode = (DataObject*) nextNode->sibling;
    }

    return lastNode;

}

static void
LogicalNode_addDataObject(LogicalNode* self, DataObject* dataObject)
{
    if (self->firstChild == NULL)
        self->firstChild = (ModelNode*) dataObject;
    else {
        DataObject* lastDataObject = LogicalNode_getLastDataObject(self);

        lastDataObject->sibling = (ModelNode*) dataObject;
    }
}

static void
LogicalNode_addReportControlBlock(LogicalNode* self, ReportControlBlock* rcb)
{
    IedModel* model = (IedModel*) self->parent->parent;

    IedModel_addReportControlBlock(model, rcb);
}

ReportControlBlock*
ReportControlBlock_create(const char* name, LogicalNode* parent, char* rptId, bool isBuffered, char*
        dataSetName, uint32_t confRef, uint8_t trgOps, uint8_t options, uint32_t bufTm, uint32_t intgPd)
{
    ReportControlBlock* self = (ReportControlBlock*) GLOBAL_MALLOC(sizeof(ReportControlBlock));

    self->name = copyString(name);
    self->parent = parent;

    if (rptId)
        self->rptId = copyString(rptId);
    else
        self->rptId = NULL;

    self->buffered = isBuffered;

    if (dataSetName)
        self->dataSetName = copyString(dataSetName);
    else
        self->dataSetName = NULL;

    self->confRef = confRef;
    self->trgOps = trgOps;
    self->options = options;
    self->bufferTime = bufTm;
    self->intPeriod = intgPd;
    self->sibling = NULL;

    LogicalNode_addReportControlBlock(parent, self);

    return self;
}

#if (CONFIG_IEC61850_SETTING_GROUPS == 1)
static void
LogicalNode_addSettingGroupControlBlock(LogicalNode* self, SettingGroupControlBlock* sgcb)
{
    IedModel* model = (IedModel*) self->parent->parent;

    IedModel_addSettingGroupControlBlock(model, sgcb);
}

SettingGroupControlBlock*
SettingGroupControlBlock_create(LogicalNode* parent, uint8_t actSG, uint8_t numOfSGs)
{
    assert(actSG <= numOfSGs); /* actSG starting with 1 */
    assert(strcmp(parent->name, "LLN0") == 0);

    SettingGroupControlBlock* self = (SettingGroupControlBlock*) GLOBAL_MALLOC(sizeof(SettingGroupControlBlock));

    self->parent = parent;
    self->actSG = actSG;
    self->numOfSGs = numOfSGs;
    self->sibling = NULL;

    LogicalNode_addSettingGroupControlBlock(parent, self);

    return self;
}
#endif /* (CONFIG_IEC61850_SETTING_GROUPS == 1) */

static void
LogicalNode_addGSEControlBlock(LogicalNode* self, GSEControlBlock* gcb)
{
    IedModel* model = (IedModel*) self->parent->parent;

    IedModel_addGSEControlBlock(model, gcb);
}

GSEControlBlock*
GSEControlBlock_create(const char* name, LogicalNode* parent, char* appId, char* dataSet, uint32_t confRef, bool fixedOffs,
        int minTime, int maxTime)
{
    GSEControlBlock* self = (GSEControlBlock*) GLOBAL_MALLOC(sizeof(GSEControlBlock));

    self->name = copyString(name);
    self->parent = parent;

    if (appId)
        self->appId = copyString(appId);
    else
        self->appId = NULL;

    if (dataSet)
        self->dataSetName = copyString(dataSet);
    else
        self->dataSetName = NULL;

    self->confRef = confRef;
    self->fixedOffs = fixedOffs;
    self->minTime = minTime;
    self->maxTime = maxTime;

    self->address = NULL;

    self->sibling = NULL;

    if (parent != NULL)
        LogicalNode_addGSEControlBlock(parent, self);

    return self;
}

static void
GSEControlBlock_addPhyComAddress(GSEControlBlock* self, PhyComAddress* phyComAddress)
{
    self->address = phyComAddress;
}

PhyComAddress*
PhyComAddress_create(GSEControlBlock* parent, uint8_t vlanPriority, uint16_t vlanId, uint16_t appId, uint8_t dstAddress[])
{
    PhyComAddress* self = (PhyComAddress*) GLOBAL_MALLOC(sizeof(PhyComAddress));

    self->vlanPriority = vlanPriority;
    self->vlanId = vlanId;
    self->appId = appId;

    memcpy(self->dstAddress, dstAddress, 6);

    if (parent != NULL)
        GSEControlBlock_addPhyComAddress(parent, self);

    return self;
}

static ModelNode*
DataObject_getLastChild(DataObject* self)
{
    ModelNode* lastNode = self->firstChild;

    ModelNode* nextNode = lastNode;

    while (nextNode != NULL) {
        lastNode = nextNode;
        nextNode = (ModelNode*) nextNode->sibling;
    }

    return lastNode;
}

static void
DataObject_addChild(DataObject* self, ModelNode* child)
{
    if (self->firstChild == NULL)
        self->firstChild = child;
    else {
        ModelNode* lastChild = DataObject_getLastChild(self);

        lastChild->sibling = child;
    }
}

DataObject*
DataObject_create(const char* name, ModelNode* parent, int arrayElements)
{
    DataObject* self = (DataObject*) GLOBAL_MALLOC(sizeof(DataObject));

    self->name = copyString(name);
    self->modelType = DataObjectModelType;
    self->elementCount = arrayElements;
    self->firstChild = NULL;
    self->parent = parent;
    self->sibling = NULL;

    if (parent->modelType == LogicalNodeModelType)
        LogicalNode_addDataObject((LogicalNode*) parent, self);
    else if (parent->modelType == DataObjectModelType)
        DataObject_addChild((DataObject*) parent, (ModelNode*) self);

    return self;
}

static ModelNode*
DataAttribute_getLastChild(DataAttribute* self)
{
    ModelNode* lastNode = self->firstChild;

    ModelNode* nextNode = lastNode;

    while (nextNode != NULL) {
        lastNode = nextNode;
        nextNode = (ModelNode*) nextNode->sibling;
    }

    return lastNode;
}

static void
DataAttribute_addChild(DataAttribute* self, ModelNode* child)
{
    if (self->firstChild == NULL)
        self->firstChild = child;
    else {
        ModelNode* lastChild = DataAttribute_getLastChild(self);

        lastChild->sibling = child;
    }
}

DataAttribute*
DataAttribute_create(const char* name, ModelNode* parent, DataAttributeType type, FunctionalConstraint fc,
        uint8_t triggerOptions, int arrayElements, uint32_t sAddr)
{
    DataAttribute* self = (DataAttribute*) GLOBAL_MALLOC(sizeof(DataAttribute));

    self->name = copyString(name);
    self->elementCount = arrayElements;
    self->modelType = DataAttributeModelType;
    self->type = type;
    self->fc = fc;
    self->firstChild = NULL;
    self->mmsValue = NULL;
    self->parent = parent;
    self->sibling = NULL;
    self->triggerOptions = triggerOptions;
    self->sAddr = sAddr;

    if (parent->modelType == DataObjectModelType)
        DataObject_addChild((DataObject*) parent, (ModelNode*) self);
    else if (parent->modelType == DataAttributeModelType)
        DataAttribute_addChild((DataAttribute*) parent, (ModelNode*) self);

    return self;
}

DataSet*
DataSet_create(const char* name, LogicalNode* parent)
{
    DataSet* self = (DataSet*) GLOBAL_MALLOC(sizeof(DataSet));

    LogicalDevice* ld = (LogicalDevice*) parent->parent;

    self->name = createString(3, parent->name, "$", name);
    self->elementCount = 0;
    self->sibling = NULL;
    self->logicalDeviceName = ld->name;
    self->fcdas = NULL;

    IedModel_addDataSet((IedModel*) ld->parent, self);

    return self;
}

int
DataSet_getSize(DataSet* self)
{
    return self->elementCount;
}

DataSetEntry*
DataSet_getFirstEntry(DataSet* self)
{
    return self->fcdas;
}

DataSetEntry*
DataSetEntry_getNext(DataSetEntry* self)
{
    return self->sibling;
}

static void
DataSet_addEntry(DataSet* self, DataSetEntry* newEntry)
{
    self->elementCount++;

    if (self->fcdas == NULL)
        self->fcdas = newEntry;
    else {
        DataSetEntry* lastEntry = self->fcdas;

        while (lastEntry != NULL) {

            if (lastEntry->sibling == NULL) {
                lastEntry->sibling = newEntry;
                break;
            }

            lastEntry = lastEntry->sibling;
        }
    }
}

DataSetEntry*
DataSetEntry_create(DataSet* dataSet, const char* variable, int index, const char* component)
{
    DataSetEntry* self = (DataSetEntry*) GLOBAL_MALLOC(sizeof(DataSetEntry));

    char variableName[130];

    strncpy(variableName, variable, 129);

    char* separator = strchr(variableName, '/');

    if (separator != NULL) {
        *separator = 0;

        self->variableName = copyString(separator + 1);
        self->logicalDeviceName = copyString(variableName);
        self->isLDNameDynamicallyAllocated = true;
    }
    else {
        self->variableName = copyString(variable);
        self->logicalDeviceName = dataSet->logicalDeviceName;
        self->isLDNameDynamicallyAllocated = false;
    }

    if (component != NULL)
        self->componentName = copyString(component);
    else
        self->componentName = NULL;

    self->index = index;

    self->sibling = NULL;

    DataSet_addEntry(dataSet, self);

    return self;
}

static void
ModelNode_destroy(ModelNode* modelNode)
{
    GLOBAL_FREEMEM(modelNode->name);

    ModelNode* currentChild = modelNode->firstChild;

    while (currentChild != NULL) {
        ModelNode* nextChild = currentChild->sibling;

        ModelNode_destroy(currentChild);

        currentChild = nextChild;
    }

    if (modelNode->modelType == DataAttributeModelType) {
        DataAttribute* dataAttribute = (DataAttribute*) modelNode;

        if (dataAttribute->mmsValue != NULL) {
            MmsValue_delete(dataAttribute->mmsValue);
            dataAttribute->mmsValue = NULL;
        }
    }

    GLOBAL_FREEMEM(modelNode);
}

void
IedModel_destroy(IedModel* model)
{
    // delete all model nodes and dynamically created strings

    /* delete all logical devices */

    LogicalDevice* ld = model->firstChild;

    while (ld != NULL) {
        GLOBAL_FREEMEM (ld->name);

        LogicalNode* ln = (LogicalNode*) ld->firstChild;

        while (ln != NULL) {
            GLOBAL_FREEMEM(ln->name);

            /* delete all data objects */

            DataObject* currentDataObject = (DataObject*) ln->firstChild;

            while (currentDataObject != NULL) {
                DataObject* nextDataObject = (DataObject*) currentDataObject->sibling;

                ModelNode_destroy((ModelNode*) currentDataObject);

                currentDataObject = nextDataObject;
            }

            LogicalNode* currentLn = ln;
            ln = (LogicalNode*) ln->sibling;

            GLOBAL_FREEMEM(currentLn);
        }


        LogicalDevice* currentLd = ld;
        ld = (LogicalDevice*) ld->sibling;

        GLOBAL_FREEMEM(currentLd);
    }

    /*  delete all data sets */

    DataSet* dataSet = model->dataSets;

    while (dataSet != NULL) {
        DataSet* nextDataSet = dataSet->sibling;

        GLOBAL_FREEMEM(dataSet->name);

        DataSetEntry* dse = dataSet->fcdas;

        while (dse != NULL) {
            DataSetEntry* nextDse = dse->sibling;

            if (dse->componentName != NULL)
                GLOBAL_FREEMEM(dse->componentName);

            GLOBAL_FREEMEM(dse->variableName);

            if (dse->isLDNameDynamicallyAllocated)
                GLOBAL_FREEMEM(dse->logicalDeviceName);

            GLOBAL_FREEMEM(dse);

            dse = nextDse;
        }

        GLOBAL_FREEMEM(dataSet);

        dataSet = nextDataSet;
    }

    /* delete all RCBs */

    ReportControlBlock* rcb = model->rcbs;

    while (rcb != NULL) {
        ReportControlBlock* nextRcb = rcb->sibling;

        GLOBAL_FREEMEM(rcb->name);

        if (rcb->rptId)
            GLOBAL_FREEMEM(rcb->rptId);

        if (rcb->dataSetName)
            GLOBAL_FREEMEM(rcb->dataSetName);

        GLOBAL_FREEMEM(rcb);

        rcb = nextRcb;
    }

    /* delete all GoCBs */

    GSEControlBlock* gcb = model->gseCBs;

    while (gcb != NULL) {
        GSEControlBlock* nextGcb = gcb->sibling;

        GLOBAL_FREEMEM(gcb->name);
        GLOBAL_FREEMEM(gcb->appId);
        GLOBAL_FREEMEM(gcb->dataSetName);

        if (gcb->address)
            GLOBAL_FREEMEM(gcb->address);

        GLOBAL_FREEMEM(gcb);

        gcb = nextGcb;
    }

    /* delete setting controls */

    SettingGroupControlBlock* sgcb = model->sgcbs;

    while (sgcb != NULL) {
        SettingGroupControlBlock* nextSgcb = sgcb->sibling;

        GLOBAL_FREEMEM(sgcb);

        sgcb = nextSgcb;
    }


    /* delete generic model parts */

    if (model->name)
        GLOBAL_FREEMEM(model->name);

    GLOBAL_FREEMEM(model);

}

