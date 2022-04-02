
#ifndef QUADTREE_H
#define QUADTREE_H

#include <memory>
#include <cfloat>
#include <set>
#include "../coordinates.hpp"
#include "../types.hpp"
#include "../osm/osm.hpp"

auto cmp = [](Coordinates a, Coordinates b) { return a.getLongitude() > b.getLongitude(); };

class AABB {
    public:
        AABB(Coordinates topLeft, Coordinates bottomRight);
        Coordinates center() const;
        double maxDimension() const;
        bool containsPoint(const Coordinates& coords) const;
        bool quadIntersects(const Coordinates& center, double radius) const;
        std::array<AABB, 4> split() const;

        friend std::ostream& operator<<(std::ostream& os, const AABB& obj);
    private:
        Coordinates topLeft, bottomRight;
};

class OrderedQuadtree {
    public:
        OrderedQuadtree(AABB boundary);
        ~OrderedQuadtree();
        void insert(const OsmNode& newPoint);
        const OsmNode* nearestNeighbor(const Coordinates& queryPoint) const;

        friend std::ostream& operator<<(std::ostream& os, const OrderedQuadtree& obj);
    private:
        struct NNResult {
            const OsmNode* point = nullptr;
            double distance = 0;
        };

        void insertRec(const OsmNode& newPoint);
        void subdivide();
        const OrderedQuadtree* selectQuadrant(const Coordinates& queryPoint) const;
        bool findNearest(const Coordinates& queryPoint, NNResult& best) const;

        const OsmNode* point;
        AABB boundary;
        OrderedQuadtree* nw = nullptr;
        OrderedQuadtree* ne = nullptr; 
        OrderedQuadtree* sw = nullptr;
        OrderedQuadtree* se = nullptr;
        std::set<Coordinates, decltype(cmp)> auxiliarList;
};

#endif // QUADTREE_H
