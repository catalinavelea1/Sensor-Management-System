#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "structs.h"

sensor **read_data(char *input_file, int *num)
{
    int nof_sensor;
    sensor *new_sens, **sensor_array;
    tire_sensor *new_tire;
    power_management_unit *new_unit;

    FILE *input = fopen(input_file, "rb");
    if (input == NULL)
    {
        printf("Failed to load %s\n", input_file);
        return NULL;
    }

    fread(&nof_sensor, sizeof(int), 1, input);
    *num = nof_sensor;
    sensor_array = malloc(nof_sensor * sizeof(sensor));
    if (!sensor_array) {
        printf("Fail\n");
        return NULL;
    }
    
    for(int i = 0; i < nof_sensor; i++) {

        new_sens = malloc(sizeof(sensor));
        if (!new_sens) {
            printf("Fail\n");
            return NULL;
        }

        fread(&new_sens->sensor_type, sizeof(int), 1, input);

        if (new_sens->sensor_type == 0) {
                new_tire = malloc(sizeof(tire_sensor));
                new_sens->sensor_data = malloc(sizeof(tire_sensor));
                if (!new_tire || !new_sens->sensor_data) {
                    printf("Failed\n");
                    return NULL;
                }
                fread(&new_tire->pressure, sizeof(float), 1, input);
                fread(&new_tire->temperature, sizeof(float), 1, input);
                fread(&new_tire->wear_level, sizeof(int), 1, input);
                fread(&new_tire->performace_score, sizeof(int), 1, input);
                memcpy(new_sens->sensor_data, new_tire, sizeof(tire_sensor));
                free(new_tire);
        }

        if (new_sens->sensor_type == 1) {
            new_unit = malloc(sizeof(power_management_unit));
            new_sens->sensor_data = malloc(sizeof(power_management_unit));
            if (!new_unit || !new_sens->sensor_data) {
                printf("Failed\n");
                return NULL;
            }
            fread(&new_unit->voltage, sizeof(float), 1, input);
            fread(&new_unit->current, sizeof(float), 1, input);
            fread(&new_unit->power_consumption, sizeof(float), 1, input);
            fread(&new_unit->energy_regen, sizeof(int), 1, input);
            fread(&new_unit->energy_storage, sizeof(int), 1, input);
            memcpy(new_sens->sensor_data, new_unit,
            sizeof(power_management_unit));
            free(new_unit);
        }

        fread(&new_sens->nr_operations, sizeof(int), 1, input);
        new_sens->operations_idxs = malloc(new_sens->nr_operations *
        sizeof(int));
        if (!new_sens->operations_idxs) {
            printf("Failed\n");
            return NULL;
        }
        for(int i = 0; i < new_sens->nr_operations; i++) {
            fread(&new_sens->operations_idxs[i], sizeof(int), 1, input);
        }

        sensor_array[i] = malloc(sizeof(sensor));
        if (!sensor_array[i]) {
            printf("Failed\n");
            return NULL;
        }
        memcpy(sensor_array[i], new_sens, sizeof(sensor));
        free(new_sens);
    }

    fclose(input);

    return sensor_array;
}

void sort_array(sensor **sensor_array, int nof_sensors)
{
    int j = 0;
    sensor **copy = malloc(nof_sensors * sizeof(sensor));
    if (!copy) {
        printf("Failed\n");
        return;
    }

    //adaugam mai intai in copie senzorii cu prioritate
    for(int i = 0; i < nof_sensors ; i++) {
        if (sensor_array[i]->sensor_type == 1) {
            copy[j] = malloc(sizeof(sensor));
            if (!copy[j]) {
                printf("Failed\n");
                return;
            }
            memcpy(copy[j], sensor_array[i], sizeof(sensor));
            j++;
        }
    }

    //senzorii de tip 0 sunt adaugati la final
    for(int i = 0; i < nof_sensors; i++) {
        if (sensor_array[i]->sensor_type == 0) {
            copy[j] = malloc(sizeof(sensor));
            if (!copy[j]) {
                printf("Failed\n");
                return;
            }
            memcpy(copy[j], sensor_array[i], sizeof(sensor));
            j++;
        }
    }

    //inlocuim elementele senzorului cu cele sortate
    for(int i = 0; i < nof_sensors; i++) {
        memcpy(sensor_array[i], copy[i], sizeof(sensor));
        free(copy[i]);
    }
    
    free(copy);
}

void print_data(sensor **sensor_array, int index)
{
    if (sensor_array[index]->sensor_type == 0) {
        printf("Tire Sensor\n");
        printf("Pressure: %.2f\n",
              ((tire_sensor *)sensor_array[index]->sensor_data)->pressure);
        printf("Temperature: %.2f\n",
              ((tire_sensor *)sensor_array[index]->sensor_data)->temperature);
        printf("Wear Level: %d%%\n",
              ((tire_sensor *)sensor_array[index]->sensor_data)->wear_level);
        if(((tire_sensor *)sensor_array[index]->sensor_data)->performace_score
          != 0)
            printf("Performance Score: %d\n",
            ((tire_sensor *)sensor_array[index]->sensor_data)
            ->performace_score);
        else
            printf("Performance Score: Not Calculated\n");
    }

    if(sensor_array[index]->sensor_type == 1) {
        printf("Power Management Unit\n");
        printf("Voltage: %.2f\n",
              ((power_management_unit *)sensor_array[index]->sensor_data)
              ->voltage);
        printf("Current: %.2f\n",
              ((power_management_unit *)sensor_array[index]->sensor_data)
              ->current);
        printf("Power Consumption: %.2f\n",
              ((power_management_unit *)sensor_array[index]->sensor_data)
              ->power_consumption);
        printf("Energy Regen: %d%%\n",
              ((power_management_unit *)sensor_array[index]->sensor_data)
              ->energy_regen);
        printf("Energy Storage: %d%%\n",
              ((power_management_unit *)sensor_array[index]->sensor_data)
              ->energy_storage);
    }
}
void free_data(sensor **sensor_array, int nof_sensor)
{
    for(int i = 0; i < nof_sensor; i++) {
        free(sensor_array[i]->sensor_data);
        free(sensor_array[i]->operations_idxs);
        free(sensor_array[i]);
    }
    free(sensor_array);
}

int clear_data(sensor **sensor_array, int nof_sensors)
{
    tire_sensor *tire;
    power_management_unit *unit;
    int j, copy_num = nof_sensors;

    for(int i = 0; i < nof_sensors; i++) {
        if (sensor_array[i]->sensor_type == 0) {
             tire = sensor_array[i]->sensor_data;
             if (tire->pressure < 19 || tire->pressure > 28 ||
                tire->temperature < 0 || tire->temperature > 120 ||
                tire->wear_level < 0 || tire->wear_level > 100) {
                free(sensor_array[i]->sensor_data);
                free(sensor_array[i]->operations_idxs);
                for(j = i; j < nof_sensors - 1; j++) {
                    memcpy(sensor_array[j], sensor_array[j + 1], sizeof(sensor));
                }
                nof_sensors--; // micsoram dimensiunea vectorului
                i--; // pe pozitia i se afla acum un nou senzor
                    // ne intoarcem cu o pozitie pentru a il verifica
            }
        }
        else {
            unit = sensor_array[i]->sensor_data;
            if (unit->current < -100 || unit->current > 100 ||
            unit->voltage < 10 || unit->voltage > 20 || unit->power_consumption
            < 0 || unit->power_consumption > 1000 || unit->energy_regen < 0 ||
            unit->energy_regen > 100 || unit->energy_storage < 0 ||
            unit->energy_storage > 100) {
                free(sensor_array[i]->sensor_data);
                free(sensor_array[i]->operations_idxs);
                for(j = i; j < nof_sensors - 1; j++) {
                    memcpy(sensor_array[j], sensor_array[j + 1], sizeof(sensor));
                }
                nof_sensors--;
                i--;
            }
        }
    }

    for(int i = nof_sensors; i < copy_num; i++)
        free(sensor_array[i]); // eliberam senzorii din vector ce au ramas
                               // in afara numarului valid de senzori
    *sensor_array = realloc(*sensor_array, nof_sensors * sizeof(sensor));
        
    return nof_sensors;
}

void analyze_data(sensor **sensor_array, int index, int nof_sensor)
{
    void *operations[8];
    get_operations(operations);

    for(int i = 0; i < sensor_array[index]->nr_operations; i++) {
        ((void (*)(void *)) operations[sensor_array[index]
        ->operations_idxs[i]])(sensor_array[index]->sensor_data);
    }
}


int main(int argc, char *argv[]) {
    char command[100];
    int index, nof_sensor;
    sensor **sensor_array = read_data(argv[1], &nof_sensor);
    sort_array(sensor_array, nof_sensor);


    while(1) {
        scanf("%s", command);

        if (strcmp(command, "print") == 0) {
            scanf("%d", &index);
            if (index < 0 || index >= nof_sensor)
                printf("Index not in range!\n");
            else
                print_data(sensor_array, index);
        }
        else if (strcmp(command, "clear") == 0) {
           nof_sensor = clear_data(sensor_array, nof_sensor);
        }
        else if (strcmp(command, "analyze") == 0) {
            scanf("%d", &index);
            if (index < 0 || index >= nof_sensor)
                printf("Index not in range!\n");
            else
                analyze_data(sensor_array, index, nof_sensor);
        }
        else if (strcmp(command, "exit") == 0) {
            free_data(sensor_array, nof_sensor);
            break;
        }
    }
}
