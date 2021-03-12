/**
 * \file TableHachage.h
 * \brief Classe définissant une table de hachage.
 * \author Ludovic Trottier
 * \author Abder
 * \version 0.3
 * \date mai 2014
 *
 *	Résolution des collisions par redistribution quadratique.
 *
 */

#ifndef TABLEHACHAGE_H_
#define TABLEHACHAGE_H_

#include <vector>

namespace labTableHachage {

    const size_t MAX_TENTATIVES = 10000 ;
/**
 * \class HashTable
 *
 * \brief classe générique représentant une table de dispersion
 *
 *  La table est implémentée dans un vector. La résolution des collisions
 *  se fait par redispersion quadratique tel que vu dans le cours.
 *
 * TypeClef : le type des clefs
 * TypeElement : le type des éléments
 * FoncteurHachage: foncteur de hachage
 */
template<typename TypeClef, typename TypeElement, class FoncteurHachage>
class TableHachage {
public:

	TableHachage(size_t = 100);

	void inserer(const TypeClef &, const TypeElement &);
	void enlever(const TypeClef &);

	bool contient(const TypeClef &) const;
	TypeElement element(const TypeClef &) const;

	void rehacher();
	void vider();
	int taille() const;

	double statistiques() const;

	void afficher(std::ostream &) const;
	template<typename TClef, typename TElement, class FHachage>
	friend std::ostream& operator <<(std::ostream &,
			const TableHachage<TClef, TElement, FHachage> &);

private:

	/**
	 * \enum EtatEntree
	 * \brief Les tags pour définir l'état d'une entrée dans la table
	 */
	enum EtatEntree {
		OCCUPE, /*!< l'entrée est occupée*/
		VACANT, /*!< l'entrée n'a jamais été utilisé*/
		EFFACE /*!< l'entrée a été utilisée mais ne l'est plus actuellement*/
	};

	/**
	 * \class EntreeHachage
	 *
	 * \brief Classe interne pour définir une entrée dans la table
	 *
	 */
	class EntreeHachage {
	public:
		TypeClef m_clef; /*!< la clé de hachage*/
		TypeElement m_el; /*!< la valeur associée à la clé*/
		EtatEntree m_info; /*!< tag pour préciser l'état de l'entrée */

		/**
		 *  \brief Constructeur par défaut
		 */
		EntreeHachage() :
				m_info(VACANT) {
		}
		/**
		 *  \brief Constructeur avec argument pour initialiser les membres de la classe
		 */
		EntreeHachage(const TypeClef & p_clef, const TypeElement & p_el,
				EtatEntree p_info = VACANT) :
				m_clef(p_clef), m_el(p_el), m_info(p_info) {
		}

		/**
		 *  \brief Surcharge de l'opérateur <<
		 */
		friend std::ostream& operator<<(std::ostream & p_out,
				const EntreeHachage & p_source) {
			p_out << "(" << p_source.m_clef << "," << p_source.m_el << ")";
			return p_out;
		}
	};
	std::vector<EntreeHachage> m_tab; /*!< La table de hachage */
	size_t m_cardinalite; /*!< Le nombre d'éléments actifs dans la table */
	static const int TAUX_MAX = 50; /*!< Taux de remplissage maximum dans la table */
	FoncteurHachage m_hachage; /*!< Foncteur de hachage */

	unsigned long m_nInsertions /*!< Nombre d'insertions au total*/;
	unsigned long m_nCollisions; /*!< Le nombre de collisions au total*/

	// Méthodes privées

	int _prochainPremier(int) const;
	bool _estPremier(int) const;

    // À titre indicatif, voici les méthodes privées de la solution

	size_t _trouverPositionLibre(const TypeClef &);

	size_t _trouverPositionClef(const TypeClef &) const;
	/*
	size_t _distribution(const TypeClef &) const;
	bool _estVacante(size_t) const;
	bool _estEffacee(size_t) const;
	bool _estOccupee(size_t) const;
	bool _clefExiste(const size_t &, const TypeClef &) const;
	bool _doitEtreRehachee() const;
	void _redimensionner();
	void _reqEntreesActives(std::vector<EntreeHachage> &) const;
	void _statistiques(const unsigned long &);
    */

};
} //Fin du namespace

#include "TableHachage.hpp"

#endif
