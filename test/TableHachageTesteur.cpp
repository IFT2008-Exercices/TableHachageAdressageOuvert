/**
 * \file TableHachageTesteur.cpp
 * \brief Tests unitaires pour la classe TableHachage
 * \author Ludovic Trottier
 * \version 0.3
 * \date mai 2014
 *
 */

#include <iostream>
#include <string>
#include <vector>
#include "../TableHachage.h"
#include "../FoncteurHachage.hpp"
#include "gtest/gtest.h"
#include "../ContratException.h"

using namespace std;
using namespace labTableHachage;

TEST(TableHachage, constructeurDefaut) {
    TableHachage<string, double, HString1> t ;
}


TEST(TableHachage, insererNoThrow) {
	TableHachage<string, double, HString1> table;
	EXPECT_NO_THROW(table.inserer("pomme", 15.3));
	EXPECT_NO_THROW(table.inserer("orange", 12.4));
	EXPECT_NO_THROW(table.inserer("fraise", 16.4));
	EXPECT_NO_THROW(table.inserer("banane", 7.23));
	EXPECT_NO_THROW(table.inserer("poire", 9.45));
	EXPECT_NO_THROW(table.inserer("mangue", 7.6));
	EXPECT_NO_THROW(table.inserer("raisin", 9.0));
	EXPECT_NO_THROW(
			table.inserer("nom de fruit inconnu mais tres savoureux", 55.0));
	table.afficher(cout) ;
}

class TableHachageTest: public ::testing::Test {
protected:
	virtual void SetUp() {
		table.inserer("pomme", 15.3);
		table.inserer("orange", 12.4);
		table.inserer("fraise", 16.4);
		table.inserer("banane", 7.23);
		table.inserer("poire", 9.45);
		table.inserer("mangue", 7.6);
		table.inserer("raisin", 9.0);
		table.inserer("nom de fruit inconnu mais tres savoureux", 55.0);
	}
	// virtual void TearDown() {}
	TableHachage<string, double, HString1> table;
};

TEST_F(TableHachageTest, insererOk) {
	EXPECT_TRUE(table.contient("pomme"));
	EXPECT_TRUE(table.contient("orange"));
	EXPECT_TRUE(table.contient("fraise"));
	EXPECT_TRUE(table.contient("banane"));
	EXPECT_TRUE(table.contient("poire"));
	EXPECT_TRUE(table.contient("mangue"));
	EXPECT_TRUE(table.contient("raisin"));
	EXPECT_TRUE(table.contient("nom de fruit inconnu mais tres savoureux"));
}


TEST_F(TableHachageTest, insererThrow) {
	EXPECT_THROW(table.inserer("pomme", 123.4), PreconditionException);
	EXPECT_THROW(table.inserer("orange", 123.4), PreconditionException);
	EXPECT_THROW(table.inserer("fraise", 123.4), PreconditionException);

	string alphanum = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#%?&*()[]{}.,;:^";
	string clef;
	int N = 10;
	srand(time(NULL));
	for (int i = 0; i < N; ++i) {
		clef.push_back(alphanum[rand() % alphanum.size()]);
	}
	cout << clef << endl;

}


TEST_F(TableHachageTest, tailleOk) {
	EXPECT_EQ(8, table.taille());
	table.inserer("cerise", 99.1);
	EXPECT_EQ(9, table.taille());
	table.enlever("pomme");
	EXPECT_EQ(8, table.taille());
}


TEST_F(TableHachageTest, elementOk) {
	EXPECT_EQ(15.3, table.element("pomme"));
	EXPECT_EQ(12.4, table.element("orange"));
	EXPECT_EQ(16.4, table.element("fraise"));
	EXPECT_EQ(7.23, table.element("banane"));
	EXPECT_EQ(9.45, table.element("poire"));
	EXPECT_EQ(7.6, table.element("mangue"));
	EXPECT_EQ(9.0, table.element("raisin"));
	EXPECT_EQ(55.0, table.element("nom de fruit inconnu mais tres savoureux"));
}


TEST_F(TableHachageTest, enleverOk) {
	EXPECT_NO_THROW(table.enlever("pomme"));
	EXPECT_TRUE(!table.contient("pomme"));
	EXPECT_NO_THROW(table.enlever("orange"));
	EXPECT_TRUE(!table.contient("orange"));
	EXPECT_NO_THROW(table.enlever("fraise"));
	EXPECT_TRUE(!table.contient("fraise"));
	EXPECT_NO_THROW(table.enlever("banane"));
	EXPECT_TRUE(!table.contient("banane"));
	EXPECT_NO_THROW(table.enlever("poire"));
	EXPECT_TRUE(!table.contient("poire"));
	EXPECT_NO_THROW(table.enlever("mangue"));
	EXPECT_TRUE(!table.contient("mangue"));
	EXPECT_NO_THROW(table.enlever("raisin"));
	EXPECT_TRUE(!table.contient("raisin"));
	EXPECT_NO_THROW(table.enlever("nom de fruit inconnu mais tres savoureux"));
	EXPECT_TRUE(!table.contient("nom de fruit inconnu mais tres savoureux"));
}


TEST_F(TableHachageTest, enleverThrowSiPasPresent) {
	TableHachage<string, double, HString1> table2;
	EXPECT_THROW(table2.enlever("patapouf"), PreconditionException);
	EXPECT_THROW(table.enlever("patapouf"), PreconditionException);
}


TEST_F(TableHachageTest, afficherOk) {
	cout << table << endl;
}


TEST_F(TableHachageTest, viderOk) {
	EXPECT_NO_THROW(table.vider());
	EXPECT_EQ(0, table.taille());
}

/*
TEST_F(TableHachageTest, rehacherOk) {
	EXPECT_NO_THROW(table.rehacher());
	EXPECT_TRUE(table.contient("pomme"));
	EXPECT_TRUE(table.contient("orange"));
	EXPECT_TRUE(table.contient("fraise"));
	EXPECT_TRUE(table.contient("banane"));
	EXPECT_TRUE(table.contient("poire"));
	EXPECT_TRUE(table.contient("mangue"));
	EXPECT_TRUE(table.contient("raisin"));
	EXPECT_TRUE(table.contient("nom de fruit inconnu mais tres savoureux"));
}

TEST(TableHachageTestIndv, fluxEnleverAjouterOk) {
	TableHachage<int, int, HInt1> table;
	srand(time(NULL));
	int v;
	for (int i = 0; i < 200000; ++i) {
		v = rand() % 3000;
		if (table.contient(v)) {
			table.enlever(v);
			EXPECT_TRUE(!table.contient(v));
		} else {
			table.inserer(v, rand() % 25);
			EXPECT_TRUE(table.contient(v));
		}
	}
	cout << "Nombre moyen de collisions par insertion: " << table.statistiques() << endl;
}
 */
