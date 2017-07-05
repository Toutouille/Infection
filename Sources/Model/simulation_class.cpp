/*
 * simulation_class.cpp
 *
 *  Created on: 30 juin 2017
 *      Author: Tristan
 */

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include "../../Includes/Model/simulation_class.h"
#include "../../Includes/Model/fireman_class.h"


/* Constructor */
Simulation::Simulation(Town* town, InfectionParameters infection_parameters)
{
	this->town = town;
	this->infection_parameters = infection_parameters;
}

/* Setters */
void Simulation::setTown(Town*& town)
{
	this->town = town;
}

void Simulation::setProbInfectSameCase(float probInfectSameCase)
{
	this->infection_parameters.prob_infect_same_case = probInfectSameCase;
}

void Simulation::setProbInfectNearCase(float probInfectNearCase)
{
	this->infection_parameters.prob_infect_near_case = probInfectNearCase;
}

void Simulation::setProbMultDead(float probMultDead)
{
	this->infection_parameters.prob_mult_dead = probMultDead;
}

void Simulation::setProbMultFireman(float probMultFireman)
{
	this->infection_parameters.prob_mult_fireman = probMultFireman;
}

void Simulation::setProbMultHospital(float probMultHospital)
{
	this->infection_parameters.prob_mult_hospital = probMultHospital;
}

void Simulation::setProbMultFireStation(float probMultFireStation)
{
	this->infection_parameters.prob_mult_fire_station = probMultFireStation;
}

/* Getters */
const Town* Simulation::getTown() const
{
	return town;
}

float Simulation::getProbInfectSameCase() const
{
	return infection_parameters.prob_infect_same_case;
}

float Simulation::getProbInfectNearCase() const
{
	return infection_parameters.prob_infect_near_case;
}

float Simulation::getProbMultDead() const
{
	return infection_parameters.prob_mult_dead;
}

float Simulation::getProbMultFireman() const
{
	return infection_parameters.prob_mult_fireman;
}

float Simulation::getProbMultHospital() const
{
	return infection_parameters.prob_mult_hospital;
}

float Simulation::getProbMultFireStation() const
{
	return infection_parameters.prob_mult_fire_station;
}

/* Methods */
void Simulation::next_round()
{
	/* TODO */
}

void Simulation::previous_round()
{
	/* TODO */
}

void Simulation::fulfill_near_case_to_move(std::vector<Case*> *vector_case_to_move, unsigned int row, unsigned int column)
{
	for(unsigned int i=std::max((int) row-1, 0); i<=std::min(row+1, this->getTown()->getHeight()-1); ++i)
	{
		for(unsigned int j=std::max((int) column-1, 0); j<=std::min(column+1, this->getTown()->getWidth()-1); ++j)
		{
			vector_case_to_move->push_back(this->getTown()->getVectorCase()[i][j]);
		}
	}
}

void Simulation::move_person_randomly(Person* person, unsigned int position_in_case)
{
	std::vector<Case*> vector_case_to_move = std::vector<Case*>();
	unsigned int rand_index = 0;
	/* Get a vector of near case where the person can move */
	fulfill_near_case_to_move(&vector_case_to_move, person->getMyCase()->getRow(), person->getMyCase()->getColumn());
	/* remove the person from the original case */
	std::vector<Person*> vector_person = person->getMyCase()->getVectorPerson();
	vector_person.erase(vector_person.begin() + position_in_case);
	person->getMyCase()->setVectorPerson(vector_person);
	/* Pick randomly a new case to move in the near non-full case */
	do
	{
		rand_index = rand()%vector_case_to_move.size();
	} while(vector_case_to_move[rand_index]->getVectorPerson().size() == vector_case_to_move[rand_index]->getNbPersonMax());
	/* Add the person in the new case */
	vector_person = vector_case_to_move[rand_index]->getVectorPerson();
	vector_person.push_back(person);
	vector_case_to_move[rand_index]->setVectorPerson(vector_person);
	/* Update my_case person attribute  */
	person->setMyCase(vector_case_to_move[rand_index]);
}

void Simulation::move_people_randomly()
{
	std::vector<unsigned int> people_moved = std::vector<unsigned int>();
	unsigned int id=0;
	srand(time(NULL));
	for(unsigned int i=0; i<this->getTown()->getHeight(); ++i)
	{
		for(unsigned int j=0; j<this->getTown()->getWidth(); ++j)
		{
			for(unsigned int k=0; k<this->getTown()->getVectorCase()[i][j]->getVectorPerson().size(); ++k)
			{
				id = this->getTown()->getVectorCase()[i][j]->getVectorPerson()[k]->getId();
				if(std::find(people_moved.begin(), people_moved.end(), id) == people_moved.end())
				{
					move_person_randomly(this->getTown()->getVectorCase()[i][j]->getVectorPerson()[k], k);
					people_moved.push_back(id);
				}
			}
		}
	}
}

void Simulation::compute_person_state(Person* person)
{
	/* TODO */
}

void Simulation::infect_by_people_in_same_case(Person* person, unsigned int position_in_case)
{
	float prob_be_infected = 0;
	Person* neighbour = NULL;
	std::vector<Person*> vector_person = person->getMyCase()->getVectorPerson();
	vector_person.erase(vector_person.begin() + position_in_case);
	/* Compute the probability to be infected by each neighbor and let the probability decide */
	for(unsigned int i=0; i<vector_person.size(); ++i)
	{
		neighbour = vector_person[i];
		prob_be_infected = 0;
		if(neighbour->getState() != Person::healthy)
		{
			prob_be_infected = this->getProbInfectSameCase();
			if(neighbour->getState() == Person::dead)
			{
				prob_be_infected *= this->getProbMultDead();
			}
			else if(dynamic_cast<Fireman*>(neighbour) != NULL)
			{
				prob_be_infected *= this->getProbMultFireman();
			}
			if(person->getMyCase()->getType() == Case::hospital)
			{
				prob_be_infected *= this->getProbMultHospital();
			}
			else if(person->getMyCase()->getType() == Case::fire_station)
			{
				prob_be_infected *= this->getProbMultFireStation();
			}
		}
		/* Let the probability decide the state of the person */
		if(static_cast <float> (rand()) / static_cast <float> (RAND_MAX) <= prob_be_infected)
		{
			person->setState(Person::sick);
			/* Stop the loop, he is sick */
			break;
		}
	}
}

void Simulation::infect_by_people_in_near_case(Person*, unsigned int position_in_case)
{
	/* TODO */
}

void Simulation::virus_propagation()
{
	Person* person = NULL;
	for(unsigned int i=0; i<this->getTown()->getHeight(); ++i)
	{
		for(unsigned int j=0; j<this->getTown()->getWidth(); ++j)
		{
			for(unsigned int k=0; k<this->getTown()->getVectorCase()[i][j]->getVectorPerson().size(); ++k)
			{
				person = this->getTown()->getVectorCase()[i][j]->getVectorPerson()[k];
				if(person->getState() == Person::healthy)
				{
					infect_by_people_in_same_case(person, k);
					infect_by_people_in_near_case(person, k);
				}
			}
		}
	}
}

void Simulation::people_action()
{
	/* TODO */
}
