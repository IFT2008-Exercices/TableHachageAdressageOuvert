#include <cmath>
#include "ContratException.h"
#include <vector>
#include <stdexcept>

namespace labTableHachage {

    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    TableHachage<TypeClef, TypeElement, FoncteurHashage>::TableHachage(size_t n) : m_tab(std::vector<EntreeHachage>(_prochainPremier(n))),
                                                                                   m_cardinalite(0),
                                                                                   m_hachage(),
                                                                                   m_nInsertions(0), m_nCollisions(0) {}


    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::inserer(const TypeClef& clef, const TypeElement& element) {
        PRECONDITION(!contient(clef)) ;
        size_t index = _trouverPositionLibre(clef) ;
        m_tab.at(index) = TableHachage::EntreeHachage(clef, element, OCCUPE) ;
        ++ m_cardinalite ;
        ++ m_nInsertions ;
        if (2 * m_cardinalite > m_tab.size()) throw std::logic_error("Il faut réallouer la table!") ;

    }

    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::enlever(const TypeClef& clef) {
        PRECONDITION(contient(clef)) ;
        size_t index = _trouverPositionClef(clef) ;
        m_tab.at(index).m_info = EFFACE ;
        -- m_cardinalite ;
    }

    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    int TableHachage<TypeClef, TypeElement, FoncteurHashage>::taille() const {
        return m_cardinalite ;
    }

    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    bool TableHachage<TypeClef, TypeElement, FoncteurHashage>::contient(const TypeClef& clef) const {
        size_t index = _trouverPositionClef(clef) ;
        return m_tab.at(index).m_info == OCCUPE ;
    }

    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    TypeElement TableHachage<TypeClef, TypeElement, FoncteurHashage>::element(const TypeClef& clef) const {
        PRECONDITION(contient(clef)) ;
        size_t index = _trouverPositionClef(clef) ;
        return m_tab.at(index).m_el ;
    }

    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::vider() {
        for (auto entree: m_tab) entree.m_info = VACANT ;
        m_cardinalite = 0 ;
    }

    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    size_t TableHachage<TypeClef, TypeElement, FoncteurHashage>::_trouverPositionLibre(const TypeClef &clef) {
        size_t index = m_hachage(clef) % m_tab.size();
        size_t i = 1;
        while (m_tab.at(index).m_info == OCCUPE) {
            ++m_nCollisions ;
            index += i * i;
            index %= m_tab.size() ;
            ++i;
            ASSERTION(i < MAX_TENTATIVES);
        }
        return index;

    }

    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    size_t TableHachage<TypeClef, TypeElement, FoncteurHashage>::_trouverPositionClef(const TypeClef &clef) const {
        size_t index = m_hachage(clef) % m_tab.size();
        size_t i = 1;
        while ((m_tab.at(index).m_clef != clef) and (m_tab.at(index).m_info != VACANT)) {
            index += i * i;
            index %= m_tab.size() ;
            ++i;
            ASSERTION(i < MAX_TENTATIVES);
        }
        return index;
    }

    template<typename TypeClef, typename TypeElement, class FoncteurHachage>
    void TableHachage<TypeClef, TypeElement, FoncteurHachage>::afficher(
            std::ostream &p_out) const {
        p_out << "{";
        for (size_t i = 0; i < m_tab.size(); ++i) {
            if (m_tab[i].m_info == OCCUPE) {
                p_out << m_tab[i] << ",";
            }
        }
        p_out << "}";
    }

    template<typename TypeClef, typename TypeElement, class FoncteurHachage>
    int TableHachage<TypeClef, TypeElement, FoncteurHachage>::_prochainPremier(
            int p_entier) const {
        if (p_entier % 2 == 0) {
            p_entier++;
        }
        while (!_estPremier(p_entier)) {
            p_entier += 2;
        }
        return p_entier;
    }

    template<typename TypeClef, typename TypeElement, class FoncteurHachage>
    bool TableHachage<TypeClef, TypeElement, FoncteurHachage>::_estPremier(
            int p_entier) const {
        if (p_entier <= 1) {
            return false;
        }
        if (p_entier == 2) {             // le seul nombre premier pair
            return true;
        }
        if (p_entier % 2 == 0) {   // sinon, ce n'est pas un nombre premier
            return false;
        }

        int divisor = 3;
        int upperLimit = static_cast<int>(sqrt((float) p_entier) + 1);

        while (divisor <= upperLimit) {
            if (p_entier % divisor == 0) {
                return false;
            }
            divisor += 2;
        }
        return true;
    }

    template<typename TClef, typename TElement, class FHachage>
    std::ostream &operator<<(std::ostream &p_out,
                             const TableHachage <TClef, TElement, FHachage> &p_source) {
        p_source.afficher(p_out);
        return p_out;
    }

} //Fin du namespace

