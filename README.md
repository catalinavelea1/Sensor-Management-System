# Sensor Management System

The project implements a sensor management system in C, designed to process
and analyze data from tire sensors and power management units. It supports
reading sensor information from binary files, sorting sensors by priority,
displaying their data, executing specific operations and removing sensors
with invalid values.

-- Reading Sensor Data --

The `read_data` function reads sensor information from a binary file and
dynamically allocates memory for each sensor.

Each sensor has a type that determines the structure of its data. Tire
sensors store information about pressure, temperature, wear level and
performance score, while power management units contain information about
voltage, current, power consumption, energy regeneration and energy storage.

For each sensor, I also store the number of operations and their
corresponding indices, which will later be used to analyze the sensor data.

-- Sorting Sensors --

The `sort_array` function sorts the sensors based on their type, giving
priority to power management units.

I first create a temporary array in which I copy all sensors of type PMU,
followed by the tire sensors. After all sensors have been added, I replace
the elements of the original array with the sorted ones.

This ensures that power management units appear before tire sensors while
preserving their relative order within each category.

-- Displaying Sensor Data --

The `print_data` function displays the information associated with a sensor
at a specified index.

Depending on the sensor type, I access the corresponding structure and
print its values.

For tire sensors, I display pressure, temperature, wear level and
performance score. If the performance score has not been calculated, an
appropriate message is displayed.

For power management units, I display voltage, current, power consumption,
energy regeneration and energy storage.

-- Removing Invalid Sensors --

The `clear_data` function checks the values stored by each sensor and
removes those that do not satisfy the required constraints.

For tire sensors, I verify that pressure, temperature and wear level are
within their valid ranges.

For power management units, I check voltage, current, power consumption,
energy regeneration and energy storage.

If a sensor contains invalid values, its associated data and operation
indices are released, and the remaining sensors are shifted to remove it
from the array.

The number of sensors is then updated.

-- Analyzing Sensor Data --

The `analyze_data` function executes the operations associated with a
specified sensor.

I use the `get_operations` function to obtain an array of function pointers,
where each position corresponds to a specific operation.

For each sensor, I iterate through its operation indices and call the
corresponding function using the sensor data as an argument.

For tire sensors, the available operations check pressure, temperature and
wear level, or calculate a performance score based on these values.

For power management units, the operations calculate power output,
regenerate energy, determine energy usage and check battery health.

-- Memory Management --

The project uses dynamic memory allocation to store sensors, their
associated data and operation indices.

Memory is allocated using `malloc`, while `memcpy` is used to copy sensor
structures when reading and sorting the data.

The `free_data` function releases all dynamically allocated memory
associated with each sensor, including its data and operation indices,
before freeing the sensor array.

-- Command Processing --

The program uses a command-based interface that allows the user to
interact with the sensors.

The `print` command displays the information of a sensor at a specified
index, while `analyze` executes its associated operations.

The `clear` command removes sensors containing invalid data and updates
the number of sensors.

For commands that require an index, I verify that it belongs to the valid
range before accessing the sensor.

The `exit` command releases the allocated memory and terminates the program.
