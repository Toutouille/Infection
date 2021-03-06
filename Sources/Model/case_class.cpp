/*
 * case_class.cpp
 *
 *  Created on: 17 juin 2017
 *      Author: Tristan
 */

#include "../../Includes/Model/case_class.h"


/* Constructors */
Case::Case(unsigned int row, unsigned int column, Case_type type, unsigned int nb_person_max, std::vector<Person*> vector_person)
{
	this->row = row;
	this->column = column;
	this->type = type;
	this->nb_person_max = nb_person_max;
	this->vector_person = vector_person;
}

/* Setters */
void Case::setRow(unsigned int row)
{
	this->row = row;
}

void Case::setColumn(unsigned int column)
{
	this->column = column;
}

void Case::setNbPersonMax(unsigned int nb_person_max)
{
	this->nb_person_max = nb_person_max;
}

void Case::setType(Case::Case_type type)
{
	this->type = type;
}

void Case::setVectorPerson(const std::vector<Person*>& vector_person)
{
	this->vector_person = vector_person;
}

/* Getters */
unsigned int Case::getRow() const
{
	return row;
}

unsigned int Case::getColumn() const
{
	return column;
}

unsigned int Case::getNbPersonMax() const
{
	return nb_person_max;
}

Case::Case_type Case::getType() const
{
	return type;
}

const std::vector<Person*>& Case::getVectorPerson() const
{
	return vector_person;
}
