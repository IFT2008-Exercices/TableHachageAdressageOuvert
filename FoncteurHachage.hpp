/**
 * \file FoncteurHachage.hpp
 * \brief Implémantation des foncteurs de hachage
 * \author Ludovic Trottier
 * \author Abder
 * \version 0.3
 * \date mai 2014
 *
 * L'objet foncteur servant au template TableHachage a les spécifications suivantes:
 *
 * Un constructeur prenant un paramètre size_t qui est la capacité maximale de la table de hachage
 * Un opérateur (clef, n) prenant un paramètre de type clef, que l'on veut hacher, et un paramètre n de type size_t
 * qui représente le nombre de tentatives de hachage faites.  Il retourne un index (hash) de la forme:
 * H(clef) = ( h(clef) + f(i) ) % module
 * où h() est la fonction de hachage primaire, et f(i) est la fonction de résolution des collisions
 */

namespace labTableHachage {
/**
 * \class HString1
 * \brief Foncteur de hachage pour des string
 */
    class HString1 {
    public:
        size_t operator()(const std::string &p_clef) const {
            size_t total = 0;
            int nPremiers = 18;
            size_t premiers[18] = {11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53,
                                   59, 61, 67, 71, 73, 79};
            for (size_t i = 0; i < p_clef.length(); i++) {
                total += premiers[i % nPremiers] * p_clef[i];
            }
            return total * GRAND_PREMIER;
        }

    private:
        static const size_t GRAND_PREMIER = 1000003;
    };


/**
 * \class HString2
 * \brief Foncteur de hachage pour des string
 */
    class HString2 {
    public:
        size_t operator()(const std::string &p_clef) const {
            size_t total = 5381;
            for (size_t i = 0; i < p_clef.size(); ++i) {
                total = total * 33 ^ p_clef[i];
            }
            return total;
        }
    };

/**
 * \class HInt1
 * \brief Foncteur de hachage pour des int
 */
    class HInt1 {
    public:
        size_t operator()(const int &p_clef) const {
            return p_clef;
        }
    };


/**
 * \class HInt2
 * \brief Foncteur de hachage pour des int
 */
    class HInt2 {
    public:
        size_t operator()(const int &p_clef) const {
            size_t total(p_clef);
            total = ((total >> 16) ^ total) * 0x45d9f3b;
            total = ((total >> 16) ^ total) * 0x45d9f3b;
            total = ((total >> 16) ^ total);
            return total;
        }
    };

    /**
 * @class HacheurQuadStr1
 *
 * Objet fonction servant à implanter une fonction de hachage ayant la forme suivante:
 *
 * H(x, i) = ( h(x) + f(i) ) % module où x est une clef à hacher, h(x) est la fonction de hachage primaire, i est le
 * nombre de tentatives de hachage, f(i) est la fonction de rehachage en cas de collision et module est la taille maximale
 * de la table de dispersion.
 *
 */
    class HacheurQuadStr1 : public HString1 {
    public:

        /**
         * @brief Constructeur
         * @param p_tailleTable La capacité maximale de la table de dispersion
         */

        HacheurQuadStr1(size_t p_tailleTable) : HString1(), module(p_tailleTable) {}

        /**
         * @brief Fonction de hachage en adressage ouvert
         * @param p_clef La clef à hacher
         * @param p_tentative Le nombre de collisions rencontré
         * @return Le hash voulu
         */
        size_t operator()(const std::string &p_clef, size_t p_tentative = 0) const {
            return (HString1::operator()(p_clef) + p_tentative * p_tentative) % module;
        }

    private:
        size_t module;
    };

    /**
 * @class HacheurQuadInt1
 *
 * Objet fonction servant à implanter une fonction de hachage ayant la forme suivante:
 *
 * H(x, i) = ( h(x) + f(i) ) % module où x est une clef à hacher, h(x) est la fonction de hachage primaire, i est le
 * nombre de tentatives de hachage, f(i) est la fonction de rehachage en cas de collision et module est la taille maximale
 * de la table de dispersion.
 *
 */
    class HacheurQuadInt1 : public HInt1 {
    public:
        /**
         * @brief Constructeur
         * @param p_tailleTable La taille maximale de la table de dispersion
         */
        HacheurQuadInt1(size_t p_tailleTable) : HInt1(), module(p_tailleTable) {}

        /**
         * @brief Surcharge de l'opérateur d'appel. Retourne le résultat H(clef) = ( h(celf) + f(p_tentative) ) % module
         * @param p_clef La clef à hacher
         * @param p_tentative Le nombre de tentatives de hachage
         * @return Le hash cherché
         */
        size_t operator()(const int &p_clef, size_t p_tentative = 0) const {
            return (HInt1::operator()(p_clef) + p_tentative * p_tentative) % module;
        }

    private:
        size_t module;
    };


} // Fin namespace
