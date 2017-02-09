#!/usr/intel/bin/python3.4.1

import random


def main():
    first_names = get_list_of_names_from_file("first_names.txt")
    last_names = get_list_of_names_from_file("last_names.txt")

    employee_database = generate_employee_database(first_names, last_names)
    write_employee_database(employee_database, "employee_info.db")

    trips_database = generate_trip_database(employee_database, 100000)
    write_trip_database(trips_database, "trips.db")


def get_list_of_names_from_file(filename):
    with open(filename, 'r') as filehandle:
        names_list = extract_list_of_names(filehandle)

    return names_list


def extract_list_of_names(text_stream):
    names_list = list()
    for line in text_stream:
        names_list.append(line.rstrip('\n'))

    return names_list


def get_random_number(min_bound, max_bound):
    return random.randrange(min_bound, max_bound)


def generate_random_name(first_names, last_names):
    number_of_first_names = len(first_names)
    number_of_last_names = len(last_names)

    first_name_index = get_random_number(0, number_of_first_names)
    last_name_index = get_random_number(0, number_of_last_names)

    name = first_names[first_name_index] + ' ' + last_names[last_name_index]
    return name


def generate_employee_database(first_names, last_names):
    employees = list()

    for employee_id in range(0,20000):
        employee_info = dict()
        employee_info['ID'] = employee_id
        employee_info['name'] = generate_random_name(first_names, last_names)
        employees.append(employee_info)

    return employees


def write_employee_database(employee_database, output_filename):
    with open(output_filename, 'w') as output_filehandle:
        for employee in employee_database:
            output_filehandle.write("{},{}\n".format(employee['ID'], employee['name']))


def generate_trip_database(employee_database, num_of_trips):
    number_of_employees = len(employee_database)
    trips = list()

    for i in range(0, num_of_trips):
        trip = dict()
        date = str(get_random_number(1, 12)).zfill(2) + '/' + str(get_random_number(1, 30)).zfill(2) + '/' + str(get_random_number(1, 17)).zfill(2)
        employee = employee_database[get_random_number(0, number_of_employees)]
        trip['date'] = date
        trip['ID'] = employee['ID']
        trip['city'] = "placeholder"
        trips.append(trip)

    return trips


def write_trip_database(trip_database, output_filename):
    with open(output_filename, 'w') as output_filehandle:
        for trip in trip_database:
            output_filehandle.write("{},{},{}\n".format(trip['date'], trip['ID'], trip['city']))


if __name__ == '__main__':
    main()

