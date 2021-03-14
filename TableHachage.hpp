#include <cmath>
#include "ContratException.h"
#include <vector>
#include <stdexcept>

namespace labTableHachage {

    /**
     * @brief Constructeur
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam Hacheur Doit être un objet-fonction Hacheur tel que décrit dans la documentation de FoncteurHachage.cpp
     * @param n La cardinalité approximative du vecteur contenant la table de dispersion.  Cette cardinalité sera en fait
     * le nombre premier suivant n.
     */
    template<typename TypeClef, typename TypeElement, class Hacheur>
    TableHachage<TypeClef, TypeElement, Hacheur>::TableHachage(size_t n) :
            m_tailleTable(_prochainPremier(n)),
            m_tab(std::vector<EntreeHachage>(m_tailleTable)),
            m_cardinalite(0),
            m_hachage(m_tailleTable),
            m_nInsertions(0), m_nCollisions(0) {}

    /**
     * @brief Ajoute une paire clef-valeur dans la table de dispersion
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param clef La clé de la paire clef-valeur
     * @param element La valeur de la paire clef-valeur
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void
    TableHachage<TypeClef, TypeElement, FoncteurHashage>::inserer(const TypeClef &clef, const TypeElement &element) {
        PRECONDITION(!contient(clef));
        size_t index = _trouverPositionLibre(clef);
        m_tab.at(index) = TableHachage::EntreeHachage(clef, element, OCCUPE);
        ++m_cardinalite;
        if (_doitEtreRehachee()) rehacher();

    }

    /**
     * @brief Retirer une paire clef-valeur de la table de dispersion
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param clef La clé de la paire clef-valeur à retirer
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::enlever(const TypeClef &clef) {
        PRECONDITION(contient(clef));
        size_t index = _trouverPositionClef(clef);
        m_tab.at(index).m_info = EFFACE;
        --m_cardinalite;
    }

    /**
     * @brief Donne le nombre d'éléments dans la table
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @return Le nombre d'éléments de la table de dispersion
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    int TableHachage<TypeClef, TypeElement, FoncteurHashage>::taille() const {
        return m_cardinalite;
    }

    /**
     * @brief Donne le taux moyen de collisions: le nombre total de collisions divisé par le nombre d'insertions
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @return Le nombre de collisions divisé par le nombre d'insertions
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    double TableHachage<TypeClef, TypeElement, FoncteurHashage>::statistiques() const {
        PRECONDITION(taille() > 0);
        return static_cast<double>(m_nCollisions) / static_cast<double>(m_nInsertions);
    }

    /**
     * @brief Vérifie la présence d'une paire clef-valeur dans la table, à partir de la clef
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param clef la clef de la paire clef-valeur cherchée
     * @return true si la clef est présente dans la table
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    bool TableHachage<TypeClef, TypeElement, FoncteurHashage>::contient(const TypeClef &clef) const {
        size_t index = _trouverPositionClef(clef);
        return m_tab.at(index).m_info == OCCUPE;
    }

    /**
     * @brief Retourne la valeur correspondant à une clef donnée
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param clef La clef de la paire clef-valeur cherchée
     * @return La valeur correspondant à la clef
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    TypeElement TableHachage<TypeClef, TypeElement, FoncteurHashage>::element(const TypeClef &clef) const {
        PRECONDITION(contient(clef));
        size_t index = _trouverPositionClef(clef);
        return m_tab.at(index).m_el;
    }

    /**
     * @brief Enlève tous les éléments de la table
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::vider() {
        for (auto &entree: m_tab) entree.m_info = VACANT;
        m_cardinalite = 0;
    }

    /**
     * @brief Agrandit la table lorsque qu'un certain taux d'occupation est atteint
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::rehacher() {
        std::vector<EntreeHachage> sauvegarde;
        _reqEntreesActives(sauvegarde);
        vider();
        _redimensionner();
        m_hachage = FoncteurHashage(m_tailleTable);
        for (auto entree: sauvegarde) inserer(entree.m_clef, entree.m_el);

    }

    /**
     * @brief Insère la liste des paires clé-valeur de la table dans un flux de sortie
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHachage
     * @param p_out Le flux de sortie
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHachage>
    void TableHachage<TypeClef, TypeElement, FoncteurHachage>::afficher(
            std::ostream &p_out) const {
        p_out << "{";
        for (size_t i = 0; i < m_tab.size(); ++i) {
            if (_estOccupee(i)) {
                p_out << m_tab[i] << ",";
            }
        }
        p_out << "}";
    }

    /**
     * @brief Opérateur d'insertion dans un flux de sortie
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHachage
     * @param p_out
     * @param p_source
     * @return
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHachage>
    std::ostream &operator<<(std::ostream &p_out,
                             const TableHachage <TypeClef, TypeElement, FoncteurHachage> &p_source) {
        p_source.afficher(p_out);
        return p_out;
    }

    /**
     * @brief Tente de trouver un hash vacant correspondant à la clef.  Recommence tant qu'une place libre n'est pas
     * trouvée.
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param clef La clef souhaitée
     * @return Un indice pointant à un endroit vacant dans la table
     * @except AssertionError si un nombre excessif de collisions est rencontré
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    size_t TableHachage<TypeClef, TypeElement, FoncteurHashage>::_trouverPositionLibre(const TypeClef &clef) {
        size_t index = m_hachage(clef, 0);
        size_t tentative = 1;
        while (_estOccupee(index)) {
            index = m_hachage(clef, tentative);
            ++tentative;
            ASSERTION(tentative < MAX_TENTATIVES);
        }
        _statistiques(tentative - 1);
        return index;

    }

    /**
     * @brief Trouve un index correspondant à une clef donnée. Recommence jusqu'à ce que la clef soit localisée ou la
     * place soit vacante.
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param clef La clef souhaitée
     * @return Un index qui: contient la clef, ou est libre.
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    size_t TableHachage<TypeClef, TypeElement, FoncteurHashage>::_trouverPositionClef(const TypeClef &clef) const {
        size_t index = m_hachage(clef, 0);
        size_t tentative = 1;
        while ((m_tab.at(index).m_clef != clef) and (!_estVacante(index))) {
            index = m_hachage(clef, tentative);
            ++tentative;
            ASSERTION(tentative < MAX_TENTATIVES);
        }
        return index;
    }

    /**
     * @brief Indique si un index donnée indique une position vacante
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param i
     * @return true si la table est vacante en position i
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    bool TableHachage<TypeClef, TypeElement, FoncteurHashage>::_estVacante(size_t i) const {
        return m_tab.at(i).m_info == VACANT;
    }

    /**
     * @brief Indique si un index donné pointe à une position effacée
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param i Un index dans la table
     * @return true si la table est effacée en position i
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    bool TableHachage<TypeClef, TypeElement, FoncteurHashage>::_estEffacee(size_t i) const {
        return m_tab.at(i).m_info == EFFACE;
    }

    /**
     * @brief Indique si un index donné pointe à une position active
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param i un index dans la table
     * @return true si la table est occupée à la position i
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    bool TableHachage<TypeClef, TypeElement, FoncteurHashage>::_estOccupee(size_t i) const {
        return m_tab.at(i).m_info == OCCUPE;
    }

    /**
     * @brief Indique si le taux d'occupation de la table est supérieur à TAUX_MAX qui est un attribut statique défini
     * dans TableHachage.h
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @return true si le taux d'occupation de la table est supérieur à TAUX_MAX
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    bool TableHachage<TypeClef, TypeElement, FoncteurHashage>::_doitEtreRehachee() const {
        return 100 * m_cardinalite > TAUX_MAX * m_tailleTable;
    }

    /**
     * @brief Retourne un vecteur contenant toutes les paires clef-valeur de la table
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param sauvegarde Vecteur contenant les entrées de la table
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::_reqEntreesActives(
            std::vector<EntreeHachage> &sauvegarde) const {
        sauvegarde.clear();
        for (auto entree: m_tab) {
            if (entree.m_info == OCCUPE) sauvegarde.push_back(entree);
        }
    }

    /**
     * @brief Agrandit la table au nombre suivant: le nombre premier suivant immédiatement le double de la taille
     * actuelle.
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::_redimensionner() {
        size_t nouvelleTaille = _prochainPremier(2 * m_tailleTable);
        m_tab.resize(nouvelleTaille);
        m_tailleTable = nouvelleTaille;
    }

    /**
     * @brief À chaque appel de _trouverPositionLibre, incrément le nombre d'insertions de 1 et le nombre de collisions
     * du nombre spécifié.
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHashage
     * @param collisions Le nombre de collisions rencontré lors de la tentative de trouver un index libre
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHashage>
    void TableHachage<TypeClef, TypeElement, FoncteurHashage>::_statistiques(const size_t &collisions) {
        m_nCollisions += collisions;
        ++m_nInsertions;
    }


    /**
     * @brief Trouve le nombre premier suivant un nombre donné
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHachage
     * @param p_entier Le nombre après lequel on veut trouver un nombre premier
     * @return Le nombre premier suivant p_entier
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHachage>
    int TableHachage<TypeClef, TypeElement, FoncteurHachage>::_prochainPremier(
            int p_entier) {
        if (p_entier % 2 == 0) {
            p_entier++;
        }
        while (!_estPremier(p_entier)) {
            p_entier += 2;
        }
        return p_entier;
    }

    /**
     * @brief Vérifie si un nombre est premier
     * @tparam TypeClef
     * @tparam TypeElement
     * @tparam FoncteurHachage
     * @param p_entier Le nombre à vérifier
     * @return true si p_entier est premier
     */
    template<typename TypeClef, typename TypeElement, class FoncteurHachage>
    bool TableHachage<TypeClef, TypeElement, FoncteurHachage>::_estPremier(
            int p_entier) {
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


} //Fin du namespace

