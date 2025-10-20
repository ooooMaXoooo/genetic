#pragma once
#include <array>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "settings.h"

template <size_t dim>
class Vec {
private:
    std::array<real, dim> m_Coordinates;

public:
    // Constructeurs
    Vec() : m_Coordinates{} {} // Initialise à zéro
    
    Vec(std::array<real, dim> coord) : m_Coordinates(coord) {}
    
    Vec(std::initializer_list<real> list) {
        if (list.size() != dim) {
            throw std::invalid_argument("Nombre d'éléments incorrect");
        }
        std::copy(list.begin(), list.end(), m_Coordinates.begin());
    }
    
    // Accesseurs
    real& operator[](size_t index) {
        return m_Coordinates[index];
    }
    
    const real& operator[](size_t index) const {
        return m_Coordinates[index];
    }
    
    real& at(size_t index) {
        if (index >= dim) {
            throw std::out_of_range("Index hors limites");
        }
        return m_Coordinates[index];
    }
    
    const real& at(size_t index) const {
        if (index >= dim) {
            throw std::out_of_range("Index hors limites");
        }
        return m_Coordinates[index];
    }
    
    size_t size() const { return dim; }
    
    // Opérateurs arithmétiques
    constexpr Vec operator+() const {
        return *this; // Unaire positif
    }
    
    constexpr Vec operator-() const {
        Vec result;
        for (size_t i = 0; i < dim; ++i) {
            result[i] = -m_Coordinates[i];
        }
        return result;
    }
    
    constexpr Vec operator+(const Vec& other) const {
        Vec result;
        for (size_t i = 0; i < dim; ++i) {
            result[i] = m_Coordinates[i] + other[i];
        }
        return result;
    }
    
    constexpr Vec operator-(const Vec& other) const {
        Vec result;
        for (size_t i = 0; i < dim; ++i) {
            result[i] = m_Coordinates[i] - other[i];
        }
        return result;
    }
    
    // Multiplication par un scalaire
    constexpr Vec operator*(real scalar) const {
        Vec result;
        for (size_t i = 0; i < dim; ++i) {
            result[i] = m_Coordinates[i] * scalar;
        }
        return result;
    }
    
    // Division par un scalaire
    constexpr Vec operator/(real scalar) const {
        if (scalar == 0) {
            throw std::invalid_argument("Division par zéro");
        }
        Vec result;
        for (size_t i = 0; i < dim; ++i) {
            result[i] = m_Coordinates[i] / scalar;
        }
        return result;
    }
    
    // Opérateurs d'assignation
    Vec& operator+=(const Vec& other) {
        for (size_t i = 0; i < dim; ++i) {
            m_Coordinates[i] += other[i];
        }
        return *this;
    }
    
    Vec& operator-=(const Vec& other) {
        for (size_t i = 0; i < dim; ++i) {
            m_Coordinates[i] -= other[i];
        }
        return *this;
    }
    
    Vec& operator*=(real scalar) {
        for (size_t i = 0; i < dim; ++i) {
            m_Coordinates[i] *= scalar;
        }
        return *this;
    }
    
    Vec& operator/=(real scalar) {
        if (scalar == 0) {
            throw std::invalid_argument("Division par zéro");
        }
        for (size_t i = 0; i < dim; ++i) {
            m_Coordinates[i] /= scalar;
        }
        return *this;
    }
    
    // Opérateurs de comparaison
    bool operator==(const Vec& other) const {
        return m_Coordinates == other.m_Coordinates;
    }
    
    bool operator!=(const Vec& other) const {
        return !(*this == other);
    }
    
    // Produit scalaire
    constexpr real dot(const Vec& other) const {
        real result = 0;
        for (size_t i = 0; i < dim; ++i) {
            result += m_Coordinates[i] * other[i];
        }
        return result;
    }
    
    // Norme
    constexpr real norm() const {
        return std::sqrt(dot(*this));
    }
    
    constexpr real normSquared() const {
        return dot(*this);
    }
    
    // Normalisation
    constexpr Vec normalized() const {
        real n = norm();
        if (isZero()) {
            throw std::runtime_error("Impossible de normaliser un vecteur nul");
        }
        return *this / n;
    }
    
    constexpr void normalize() {
        *this = normalized();
    }
    
    // Distance
    constexpr real distance(const Vec& other) const {
        return (*this - other).norm();
    }
    
    constexpr real distanceSquared(const Vec& other) const {
        return (*this - other).normSquared();
    }
    
    // Itérateurs
    auto begin() { return m_Coordinates.begin(); }
    auto end() { return m_Coordinates.end(); }
    auto begin() const { return m_Coordinates.begin(); }
    auto end() const { return m_Coordinates.end(); }
    
    // Affichage
    friend std::ostream& operator<<(std::ostream& os, const Vec& vec) {
        os << "(";
        for (size_t i = 0; i < dim; ++i) {
            os << vec[i];
            if (i < dim - 1) os << ", ";
        }
        os << ")";
        return os;
    }



    // méthodes utiles

    // Vecteur unitaire selon un axe
    static Vec unit(size_t axis) {
        if (axis >= dim) {
            throw std::out_of_range("Axe invalide");
        }
        Vec result;
        result[axis] = 1.0;
        return result;
    }

    // Vérifier si le vecteur est proche de zéro
    constexpr bool isZero(real epsilon = 1e-10) const {
        return normSquared() < epsilon * epsilon;
    }

    // Projection sur un autre vecteur
    constexpr Vec projectOn(const Vec& other) const {
        real otherNormSq = other.normSquared();
    
        // Utilise une petite tolérance pour la vérification
        if (std::abs(otherNormSq) < 1e-10) { 
            return Vec(); // Projection sur un vecteur nul est un vecteur nul
        }
        return other * (dot(other) / other.normSquared());
    }






    // Spécialisations

    // dim = 3

    template <size_t D = dim>
    constexpr std::enable_if_t<D == 3, Vec> cross(const Vec& other) const {
        return Vec({
            m_Coordinates[1] * other[2] - m_Coordinates[2] * other[1],
            m_Coordinates[2] * other[0] - m_Coordinates[0] * other[2],
            m_Coordinates[0] * other[1] - m_Coordinates[1] * other[0]
        });
    }
};

// Opérateur scalaire * vecteur (ordre inversé)
template <size_t dim>
constexpr Vec<dim> operator*(real scalar, const Vec<dim>& vec) {
    return vec * scalar;
}



// Alias utiles
using Vec2 = Vec<2>;
using Vec3 = Vec<3>;
using Vec4 = Vec<4>;



inline Vec3 makeVec3(real x, real y, real z) {
    return Vec3({x, y, z});
}