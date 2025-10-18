/*
 * Copyright 2025 SUCHAI Flight Software v2 project and contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "storage.h"
#include "osMalloc.h"

///< Status variables buffer
static value32_t *status_db = NULL;
static size_t status_entries = 0;

///< Flight plan buffer
static fp_entry_t *flightplan_db = NULL;
static size_t flightplan_entries = 0;

///< Payloads storage buffer
static uint8_t *payload_db = NULL;
static uint8_t **payloads_sections_addresses = NULL;
static int payloads_entries = 0;

static int storage_is_open = 0;

int storage_init(const char *db_name)
{
    storage_is_open = 1;
    return SCH_ST_OK;
}

int storage_close(void)
{
    storage_is_open = 0;
    status_entries = 0;
    flightplan_entries = 0;
    payloads_entries = 0;
    
    if(status_db != NULL) { os_free(status_db); status_db = NULL; }
    if(flightplan_db != NULL) { os_free(flightplan_db); flightplan_db = NULL; }
    if(payload_db != NULL) { os_free(payload_db); payload_db = NULL; }
    if(payloads_sections_addresses != NULL) { os_free(payloads_sections_addresses); payloads_sections_addresses = NULL; }

    return SCH_ST_OK;
}

int storage_table_status_init(char *table, int n_variables, int drop)
{
    if(!storage_is_open)
        return SCH_ST_ERROR;

    if(status_db != NULL) {
        if (drop) {os_free(status_db); status_db = NULL;}
        else return SCH_ST_ERROR;
    }

    // Status var storage
    status_entries = n_variables;
    status_db = (value32_t *)os_malloc(status_entries*sizeof(value32_t));
    if(status_db == NULL) return SCH_ST_ERROR;

    // Reset variables (we do not have persistent storage here)
    memset(status_db, 0, status_entries*sizeof(value32_t));
    return SCH_ST_OK;
}

int storage_table_flight_plan_init(char *table, int n_entries, int drop)
{
    if(!storage_is_open)
        return SCH_ST_ERROR;

    if(flightplan_db != NULL) {
        if (drop) {os_free(flightplan_db); flightplan_db = NULL;}
        else return SCH_ST_ERROR;
    }

    flightplan_entries = n_entries;
    flightplan_db = (fp_entry_t *)os_malloc(flightplan_entries * sizeof(fp_entry_t));
    if(flightplan_db == NULL) return SCH_ST_ERROR;

    //Clear internal flight plan table (cannot load a stored FP)
    int i;
    for(i=0;i<flightplan_entries;i++)
    {
        flightplan_db[i].cmd = NULL;
        flightplan_db[i].args = NULL;
        fp_entry_clear(flightplan_db + i);
    }

    return SCH_ST_OK;
}

int storage_table_payload_init(char *table, data_map_t *data_map, int n_entries, int drop)
{
    if(!storage_is_open)
        return SCH_ST_ERROR;

    if(payload_db != NULL) {
        if (drop) {os_free(payload_db); payload_db = NULL;}
        else return SCH_ST_ERROR;
    }

    payloads_entries = n_entries;
    payload_db = (uint8_t *)os_malloc(SCH_STORAGE_BLOCK_SIZE * SCH_SECTIONS_PER_PAYLOAD * payloads_entries);
    if(payload_db == NULL) return SCH_ST_ERROR;

    // Clear buffer
    memset(payload_db, 0, SCH_STORAGE_BLOCK_SIZE * SCH_SECTIONS_PER_PAYLOAD * payloads_entries);
    // Init payload sections pointers storage
    payloads_sections_addresses = (uint8_t **)os_malloc(SCH_SECTIONS_PER_PAYLOAD * payloads_entries * sizeof(uint8_t *));
    // Save the starting address corresponding to each payload memory section
    int i;
    for (i = 0; i < SCH_SECTIONS_PER_PAYLOAD * payloads_entries; i++)
        payloads_sections_addresses[i] = payload_db + i*SCH_STORAGE_BLOCK_SIZE;


    return SCH_ST_OK;
}

/****** STATUS VARIABLES FUNCTIONS *******/

int storage_status_get_value_idx(uint32_t index, value32_t *value, char *table)
{
    if(!storage_is_open || status_db == NULL)
        return SCH_ST_ERROR;

    if(index > status_entries)
        return SCH_ST_ERROR;

    *value = status_db[index];
    return SCH_ST_OK;
}

int storage_status_set_value_idx(int index, value32_t value, char *table)
{
    if(!storage_is_open || status_db == NULL)
        return SCH_ST_ERROR;

    status_db[index] = value;
    return SCH_ST_OK;
}

/****** FLIGHT PLAN VARIABLES FUNCTIONS *******/
/* TODO: Use the concept of table */
//int storage_flight_plan_set_st(fp_entry_t *row, char *table)
int storage_flight_plan_set_st(fp_entry_t *row)
{
    if(!storage_is_open || flightplan_db == NULL || row == NULL)
        return SCH_ST_ERROR;

    int i;
    for(i = 0; i < flightplan_entries; i++)
    {
        if(flightplan_db[i].unixtime == ST_FP_NULL)
        {
            fp_entry_copy(row, flightplan_db + i);
            return SCH_ST_OK;
        }
    }

    return SCH_ST_ERROR;
}

int storage_flight_plan_set(int timetodo, char* command, char* args, int executions, int period, int node)
{
    if(timetodo < 0 || command == NULL || args == NULL)
        return SCH_ST_ERROR;

    fp_entry_t row;
    row.unixtime = timetodo;
    row.executions = executions;
    row.periodical = period;
    row.node = node;
    // We do not copy here, they are copied in storage_flight_plan_set_st
    // Note that the pointers are still valid here
    row.cmd = command;
    row.args = args;

    return storage_flight_plan_set_st(&row);
}

int storage_flight_plan_get(int timetodo, fp_entry_t *row)
{
    if(!storage_is_open || flightplan_db == NULL || row == NULL)
        return SCH_ST_ERROR;

    int i;
    for(i = 0; i < flightplan_entries; i++)
    {
        if(timetodo == flightplan_db[i].unixtime)
        {
            fp_entry_copy(flightplan_db + i, row);
            return SCH_ST_OK;
        }
    }

    return SCH_ST_ERROR;
}

int storage_flight_plan_get_idx(int index, fp_entry_t *row)
{
    if(!storage_is_open || flightplan_db == NULL || row == NULL || index >= flightplan_entries)
        return SCH_ST_ERROR;

    fp_entry_copy(flightplan_db + index, row);
    return SCH_ST_OK;
}

int storage_flight_plan_get_args(int timetodo, char* command, char* args, int* executions, int* period, int* node)
{
    if(command == NULL || args == NULL || executions == NULL || period == NULL || node == NULL)
        return SCH_ST_ERROR;

    fp_entry_t fp_entry;
    int rc = storage_flight_plan_get(timetodo, &fp_entry);
    if(rc != SCH_ST_OK)
        return rc;

    strcpy(command, fp_entry.cmd);
    strcpy(args, fp_entry.args);
    *executions = fp_entry.executions;
    *period = fp_entry.periodical;
    *node = fp_entry.node;
    fp_entry_clear(&fp_entry);

    return SCH_ST_OK;
}

int storage_flight_plan_delete_row(int timetodo)
{
    if(!storage_is_open || flightplan_db == NULL)
        return SCH_ST_ERROR;

    int i;
    for(i=0; i < flightplan_entries; i++)
    {
        if(timetodo == flightplan_db[i].unixtime)
        {
            fp_entry_clear(flightplan_db + i);
            return SCH_ST_OK;
        }
    }
    return SCH_ST_ERROR;
}

int storage_flight_plan_delete_row_idx(int index)
{
    if(!storage_is_open || flightplan_db == NULL)
        return SCH_ST_ERROR;
    if(index >= flightplan_entries)
        return SCH_ST_ERROR;

    fp_entry_clear(flightplan_db + index);
    return SCH_ST_OK;
}

int storage_flight_plan_reset(void)
{
    if(!storage_is_open || flightplan_db == NULL)
        return SCH_ST_ERROR;

    int i;
    for(i=0; i < flightplan_entries; i++)
    {
        fp_entry_clear(flightplan_db + i);
        return SCH_ST_OK;
    }
    return SCH_ST_ERROR;
}

/****** PAYLOAD STORAGE FUNCTIONS *******/

/**
 * Auxiliary function to calculate payload sample destination address
 */
int _get_sample_address(int payload, int index, size_t size, uint8_t **address)
{
    int samples_per_section = SCH_STORAGE_BLOCK_SIZE / size;
    int sample_section = index / samples_per_section;
    int index_in_section = index % samples_per_section;
    int section_index = payload*SCH_SECTIONS_PER_PAYLOAD + sample_section;
    if(sample_section > SCH_SECTIONS_PER_PAYLOAD)
        return SCH_ST_ERROR;
    if(section_index > payloads_entries * SCH_STORAGE_BLOCK_SIZE)
        return SCH_ST_ERROR;

    *address = payloads_sections_addresses[section_index] + index_in_section * size;
    if(*address > payload_db + payloads_entries * SCH_SECTIONS_PER_PAYLOAD * SCH_STORAGE_BLOCK_SIZE)
        return SCH_ST_ERROR;

    return SCH_ST_OK;
}

int storage_payload_set_data(int payload, int index, void *data, data_map_t *schema)
{
    if(!storage_is_open) return SCH_ST_ERROR;
    if(payload_db == NULL || payloads_sections_addresses == NULL) return SCH_ST_ERROR;
    if(data == NULL || schema == NULL) return SCH_ST_ERROR;

    uint8_t *sample_address;
    int rc = _get_sample_address(payload, index, schema->size, &sample_address);
    if(rc != SCH_ST_OK)
        return SCH_ST_ERROR;

    memcpy(sample_address, data, schema->size);
    return SCH_ST_OK;
}

int storage_payload_get_data(int payload, int index, void *data, data_map_t *schema)
{
    if(!storage_is_open) return SCH_ST_ERROR;
    if(payload_db == NULL || payloads_sections_addresses == NULL) return SCH_ST_ERROR;
    if(data == NULL || schema == NULL) return SCH_ST_ERROR;

    uint8_t *sample_address;
    int rc = _get_sample_address(payload, index, schema->size, &sample_address);
    if(rc != SCH_ST_OK)
        return SCH_ST_ERROR;

    memcpy(data, sample_address, schema->size);
    return SCH_ST_OK;
}

int storage_payload_reset(data_map_t data_map[])
{
    return storage_table_payload_init(NULL, NULL, payloads_entries, 1);
}

int storage_payload_reset_table(int payload, data_map_t* data_map)
{
    if(!storage_is_open || payload >= payloads_entries) return SCH_ST_ERROR;
    if(payload_db == NULL || payloads_sections_addresses == NULL) return SCH_ST_ERROR;

    uint8_t *payload_address = payloads_sections_addresses[payload*SCH_SECTIONS_PER_PAYLOAD];
    size_t payload_size = SCH_STORAGE_BLOCK_SIZE*SCH_SECTIONS_PER_PAYLOAD;
    memset(payload_address, 0, payload_size);

    return SCH_ST_OK;
}
