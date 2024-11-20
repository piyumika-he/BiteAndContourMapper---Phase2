/**
 * @brief The Datastructures class
 * STUDENTS: Modify the code below to implement the functionality of the class.
 * Also remove comments from the parameter names when you implement an operation
 * (Commenting out parameter name prevents compiler from warning about unused
 * parameters on operations you haven't yet implemented.)
 */

#include "datastructures.hh"
#include "customtypes.hh"

Datastructures::Datastructures()
{
  // Write any initialization you need here
}

Datastructures::~Datastructures()
{
  // Write any cleanup you need here
}

unsigned int Datastructures::get_bite_count()
{
  return bites.size();
}

void Datastructures::clear_all()
{
    bites.clear();
    contours.clear();
    connections.clear();
    bite_ids.clear();
    coord_to_bite.clear();
    alphabetical_bites.clear();
    distance_sorted_bites.clear();
}

std::vector<BiteID> Datastructures::all_bites()
{
  return bite_ids;
}

bool Datastructures::add_bite(BiteID id, const Name & name, Coord xy)
{
    //Checks whether the bite with given id exists or a bite already exists at the given coordinates
    if (bites.find(id) != bites.end() || coord_to_bite.find(xy) != coord_to_bite.end()) {
            return false;
        }

    // Adding the bite
    bites.emplace(id, Bite{name, xy, NO_CONTOUR});
    coord_to_bite.emplace(xy, id);
    bite_ids.push_back(id);

    // Find the correct position to insert the new bite in the sorted vector
    auto it = std::lower_bound(alphabetical_bites.begin(), alphabetical_bites.end(), id,
                                   [this](BiteID a, BiteID b) {
                                       if (bites[a].name == bites[b].name) {
                                           return a < b; // Sort by id if names are the same
                                       }
                                       return bites[a].name < bites[b].name; // Sort by name
                                   });

        // Insert at the correct position to maintain sorted order
        alphabetical_bites.insert(it, id);



        auto dist_it = std::lower_bound(distance_sorted_bites.begin(), distance_sorted_bites.end(), id,
                                             [this, &xy](BiteID a, BiteID b) {
                                                 // Calculate distance for bite a
                                                 int dist_a = std::abs(bites[a].coord.x) + std::abs(bites[a].coord.y);
                                                 // Calculate distance for bite b
                                                 int dist_b = std::abs(bites[b].coord.x) + std::abs(bites[b].coord.y);

                                                 // Calculate distance for the new bite
                                                 int new_distance = std::abs(xy.x) + std::abs(xy.y);

                                                 // Compare by distance, then by y-coordinate, then by ID
                                                 if (new_distance != dist_a && new_distance != dist_b) {
                                                     return new_distance < dist_a; // Comparison against existing bite distances
                                                 } else if (dist_a != dist_b) {
                                                     return dist_a < dist_b;
                                                 } else if (bites[a].coord.y != bites[b].coord.y) {
                                                     return bites[a].coord.y < bites[b].coord.y;
                                                 }
                                                 return a < b; // Sort by id if distances and y-coordinates are the same
                                             });

        // Insert at the correct position to maintain sorted order
        distance_sorted_bites.insert(dist_it, id);


    return true;
}

Name Datastructures::get_bite_name(BiteID id)
{
    // Find a bite with a given id
    auto it = bites.find(id);
    return (it != bites.end()) ? it->second.name : NO_NAME;

}

Coord Datastructures::get_bite_coord(BiteID id)
{
    //Find the cordinates of a given bite.
    auto it = bites.find(id);
    return (it != bites.end()) ? it->second.coord : NO_COORD;

}

std::vector<BiteID> Datastructures::get_bites_alphabetically()
{
  return alphabetical_bites;
}

std::vector<BiteID> Datastructures::get_bites_distance_increasing()
{
  return distance_sorted_bites;
}

BiteID Datastructures::find_bite_with_coord(Coord xy)
{
    auto it = coord_to_bite.find(xy);
    if (it != coord_to_bite.end()) {
        return it->second;
    }
    return NO_BITE;
}

bool Datastructures::add_contour(ContourID id, const Name & name, ContourHeight height,
                                 std::vector<Coord> coords)
{
    // Check if the contour ID is unique
        if (contours.find(id) != contours.end()) {
            return false;
        }

        // Check if the height is within the valid range
        if (height > MAX_CONTOUR_HEIGHT || height < -MAX_CONTOUR_HEIGHT) {
            return false;
        }

        // Check if the coordinates list is empty
        if (coords.empty()) {
            return false;
        }

        // If all checks passed, add the contour to the data structure
        contours[id] = {name, height, coords, NO_CONTOUR, {}};

        return true;
}

std::vector<ContourID> Datastructures::all_contours()
{
    std::vector<ContourID> ids; // Initializing a Vector

    // Iterate over the contours map and collect keys
    for (const auto& [id, _] : contours) {
        ids.push_back(id);
    }

    return ids; // Return the vector of ID
}

Name Datastructures::get_contour_name(ContourID id)
{
    auto it = contours.find(id); // find the contour by ID
    if (it != contours.end()) {
        return it->second.name;
    } else {
        return NO_NAME;
    }
}

std::vector<Coord> Datastructures::get_contour_coords(ContourID id)
{
    auto it = contours.find(id); // find the contour by ID

    if (it != contours.end()) {
        return it->second.coords;
    } else {
        return std::vector<Coord>{NO_COORD};
    }
}

ContourHeight Datastructures::get_contour_height(ContourID id)
{
    auto it = contours.find(id); // Find the contour by ID

    if (it != contours.end()) {
        return it->second.height;
    } else {
        return NO_CONTOUR_HEIGHT;
    }
}

bool Datastructures::add_subcontour_to_contour(ContourID childId,
                                               ContourID parentId)
{
    auto parentIt = contours.find(parentId); // Find the parent contour
    auto childIt = contours.find(childId); // Find the child contour

    // Check if both contours exist
    if (parentIt == contours.end() || childIt == contours.end()) {
        return false;
    }

    // Check if the child contour already has a parent
    if (childIt->second.parent_id != NO_CONTOUR) {
        return false;
    }

    // heights
    ContourHeight parentHeight = parentIt->second.height;
    ContourHeight childHeight = childIt->second.height;

    // Check conditions
    if (abs(childHeight) <= abs(parentHeight)) {
        return false;
    }
    if (std::abs(childHeight - parentHeight) != 1) {

        return false;
    }

    parentIt->second.sub_contours.push_back(childId);
    childIt->second.parent_id = parentId;

    return true;
}

bool Datastructures::add_bite_to_contour(BiteID biteId, ContourID contourId)
{
    // Check if the contour exists
    auto contourIt = contours.find(contourId);
    if (contourIt == contours.end()) {
        return false;
    }

    // Check if the bite exists
    auto biteIt = bites.find(biteId);
    if (biteIt == bites.end()) {
        return false;
    }

    // Check if the bite already belongs to another contour
    if (biteIt->second.contour_id != NO_CONTOUR) {
        return false;
    }

    // Check if the bite coordinates are part of the specified contour
    const auto& contourCoords = contourIt->second.coords;
    const Coord& biteCoord = biteIt->second.coord;

    // Check if the bite coordinates match any coordinates in the contour
    if (std::find(contourCoords.begin(), contourCoords.end(), biteCoord) == contourCoords.end()) {
        return false;
    }


    //Else
    contourIt->second.sub_contours.push_back(biteId);
    biteIt->second.contour_id = contourId; // Update bite's contour_id
    return true;
}

std::vector<ContourID> Datastructures::get_bite_in_contours(BiteID id)
{
    // Check if the bite exists in the data structure
        auto biteIt = bites.find(id);
        if (biteIt == bites.end()) {

            return {NO_CONTOUR};
        }


        ContourID current_contour = biteIt->second.contour_id;
        if (current_contour == NO_CONTOUR) {

            return {};
        }

        // adding contour IDs to the result
        std::vector<ContourID> contour_chain;
        while (current_contour != NO_CONTOUR) {
            contour_chain.push_back(current_contour);
            auto contourIt = contours.find(current_contour);

            // if we can't find the parent contour, break the loop
            if (contourIt == contours.end()) {
                break;
            }


            current_contour = contourIt->second.parent_id;
        }

        return contour_chain;
}

std::vector<ContourID>
Datastructures::all_subcontours_of_contour(ContourID id)
{
    // If the contour with the given ID does not exist
    if (contours.find(id) == contours.end()) {
        return {NO_CONTOUR};
    }

    // to store all sub-contours
    std::vector<ContourID> subcontours;

    // Helper function
    std::function<void(ContourID)> collect_subcontours = [&](ContourID current_id) {
        // current contour
        const Contour& current_contour = contours[current_id];


        for (ContourID sub_id : current_contour.sub_contours) {
            subcontours.push_back(sub_id);
            collect_subcontours(sub_id);
        }
    };

    collect_subcontours(id);

    return subcontours;
}

ContourID
Datastructures::get_closest_common_ancestor_of_contours(ContourID id1,
                                                        ContourID id2)
{
    // Find the contours with the given IDs
        auto it1 = contours.find(id1);
        auto it2 = contours.find(id2);
        if (it1 == contours.end() || it2 == contours.end()) {
            return NO_CONTOUR;
        }

        // Helper function
        auto get_ancestors = [&](ContourID id) -> std::vector<ContourID> {
            std::vector<ContourID> ancestors;
            ContourID current = id;
            while (true) {
                auto it = contours.find(current);
                if (it == contours.end() || it->second.parent_id == NO_CONTOUR) {
                    break;
                }
                current = it->second.parent_id;
                ancestors.push_back(current);
            }
            return ancestors;
        };

        // Get the ancestors of both contours
        std::vector<ContourID> ancestors1 = get_ancestors(id1);
        std::vector<ContourID> ancestors2 = get_ancestors(id2);

        if (ancestors1.empty() || ancestors2.empty()) {
            return NO_CONTOUR;
        }

        // closest to farthest
        for (auto ancestor1 : ancestors1) {
            for (auto ancestor2 : ancestors2) {
                if (ancestor1 == ancestor2) {
                    return ancestor1;
                }
            }
        }

        // If no common ancestor is found
        return NO_CONTOUR;
}

bool Datastructures::remove_bite(BiteID id)
{
    // Check if the bite exists in the map
       auto it = bites.find(id);
       if (it == bites.end()) {
           return false;  // Bite does not exist, return false
       }

       // Get the corresponding coordinate of the bite
       Coord coord = it->second.coord;

       // Remove all connections associated with this bite
           for (auto conn_it = connections.begin(); conn_it != connections.end();) {
               const Connection& connection = conn_it->second;
               if (connection.start == id || connection.end == id) {
                   conn_it = connections.erase(conn_it); // Remove connection and move iterator
               } else {
                   ++conn_it; // Move to the next connection
               }
           }

       // Remove from the bites map
       bites.erase(it);

       // Remove from the coord_to_bite map
       coord_to_bite.erase(coord);

       // Remove from the bite_ids vector
       bite_ids.erase(std::remove(bite_ids.begin(), bite_ids.end(), id), bite_ids.end());

       // Remove from the alphabetical_bites vector
       alphabetical_bites.erase(std::remove(alphabetical_bites.begin(), alphabetical_bites.end(), id), alphabetical_bites.end());

       // Remove from the distance_sorted_bites vector
       distance_sorted_bites.erase(std::remove(distance_sorted_bites.begin(), distance_sorted_bites.end(), id), distance_sorted_bites.end());

       return true;
}

std::vector<BiteID> Datastructures::get_bites_closest_to(Coord xy)
{
    // If there are no bites, return an empty vector
        if (bites.empty()) {
            return {};
        }

        // Intialize a vector to store distances
        std::vector<std::tuple<BiteID, int, Coord>> bite_distances;

        // Manhattan distance
        for (const auto& [id, bite] : bites) {
            int distance = std::abs(bite.coord.x - xy.x) + std::abs(bite.coord.y - xy.y);
            bite_distances.push_back({id, distance, bite.coord});
        }

        // Sort the bites
        std::sort(bite_distances.begin(), bite_distances.end(),
            [](const std::tuple<BiteID, int, Coord>& a, const std::tuple<BiteID, int, Coord>& b) {
                int dist_a = std::get<1>(a);
                int dist_b = std::get<1>(b);

                if (dist_a != dist_b) {
                    return dist_a < dist_b;  //by distance first
                }

                Coord coord_a = std::get<2>(a);
                Coord coord_b = std::get<2>(b);

                if (coord_a.y != coord_b.y) {
                    return coord_a.y < coord_b.y;  // by y-coordinate if distances are the same
                }

                return std::get<0>(a) < std::get<0>(b);  // by BiteID if y-coordinates are the same
            }
        );

        // Extract the IDs of up to three closest bites
        std::vector<BiteID> closest_bites;
        for (size_t i = 0; i < std::min<size_t>(3, bite_distances.size()); ++i) {
            closest_bites.push_back(std::get<0>(bite_distances[i]));
        }

        return closest_bites;
}

/**
 * ============================
 * ============================
 * ========== PRG2 ============
 * ============================
 * ============================
 */

bool Datastructures::add_connection(ConnectionID connectionid, BiteID id1, BiteID id2,
                                    std::vector<Coord> xy)
{

        if (connections.find(connectionid) != connections.end()) {
            return false;
        }

        // Check if both bites exist
        if (bites.find(id1) == bites.end() || bites.find(id2) == bites.end()) {
            return false;
        }

        // Check for diagonal movement in the provided coordinates
        for (size_t i = 1; i < xy.size(); ++i) {
            if (is_diagonal(xy[i - 1], xy[i])) {
                return false;
            }
        }

        // If no diagonal movement
        //Connection new_connection = {id1, id2, xy};

        // Add connection
        connections[connectionid] = {id1, id2, xy};

        return true;
}

void Datastructures::clear_connections()
{
   connections.clear();
}

std::vector<BiteID> Datastructures::get_next_bites_from(BiteID id)
{
    // Check if the bite ID is valid
    if (bites.find(id) == bites.end()) {
        return {NO_BITE};
    }

    std::unordered_set<BiteID> connected_bites;

    // Iterate over all connections
    for (const auto& [connection_id, connection] : connections) {
        if (connection.start == id) {
            connected_bites.insert(connection.end);
        } else if (connection.end == id) {
            connected_bites.insert(connection.start);
        }
    }

    // Prepare the result vector
    std::vector<BiteID> result(connected_bites.begin(), connected_bites.end());

    return result;

}

std::vector<ConnectionID> Datastructures::get_connections(BiteID id1, BiteID id2)
{
    //(void)id1; // TODO: remove, a trick to get rid of the warning
    //(void)id2; // TODO: remove, a trick to get rid of the warning

    std::vector<ConnectionID> result;

    // No parameters
    if (id1 == NO_BITE && id2 == NO_BITE) {
        for (const auto& [connection_id, connection] : connections) {
            result.push_back(connection_id);
        }
        return result;
    }

    // One bite
    if (id2 == NO_BITE) {
        if (bites.find(id1) == bites.end()) {
            return {NO_CONNECTION};
        }

        for (const auto& [connection_id, connection] : connections) {
            if (connection.start == id1 || connection.end == id1) {
                result.push_back(connection_id);
            }
        }
        return result;
    }

    // Two bites
    if (bites.find(id1) == bites.end() || bites.find(id2) == bites.end()) {
        return {NO_CONNECTION};
    }

    // Retrieve connections specifically between the two bites
    for (const auto& [connection_id, connection] : connections) {
        if ((connection.start == id1 && connection.end == id2) ||
            (connection.start == id2 && connection.end == id1)) {
            result.push_back(connection_id);
        }
    }

    return result;

}

std::vector<Coord> Datastructures::get_connection_coords(BiteID biteid,
                                                         ConnectionID connectionid)
{
    // Check if the connection exists
        auto connection_itr = connections.find(connectionid);
        if (connection_itr == connections.end()) {
            return {NO_COORD};
        }

        const Connection& connection = connection_itr->second;

        // Check if the bite is part of the connection
        if (connection.start != biteid && connection.end != biteid) {
            return {NO_COORD};
        }

        std::vector<Coord> result;

        // If the bite is the start of the connection, return the path as it is
        if (connection.start == biteid) {
            result.push_back(bites[connection.start].coord);
            result.insert(result.end(), connection.path.begin(), connection.path.end());
            result.push_back(bites[connection.end].coord);
        }
        // If the bite is the end of the connection, return the path in reverse order
        else if (connection.end == biteid) {
            result.push_back(bites[connection.end].coord);
            result.insert(result.end(), connection.path.rbegin(), connection.path.rend());
            result.push_back(bites[connection.start].coord);
        }

        return result;
}

std::vector<std::pair<Coord, Distance>> Datastructures::path_any(BiteID fromid, BiteID toid)
{
    // Check if both BiteIDs exist
    if (bites.find(fromid) == bites.end() || bites.find(toid) == bites.end()) {
        return { {NO_COORD, NO_DISTANCE} };
    }

    // If fromid and toid are the same
    if (fromid == toid) {
        return {};
    }

    // to store visited BiteIDs
    std::unordered_map<BiteID, std::pair<BiteID, Distance>> came_from;
    std::unordered_set<BiteID> visited;

    // BFS queue
    std::queue<std::tuple<BiteID, Distance, std::vector<std::pair<Coord, Distance>>>> q;


    q.push({fromid, 0, { {bites[fromid].coord, 0} } });
    visited.insert(fromid);

    while (!q.empty()) {
        auto [current, current_dist, current_path] = q.front();
        q.pop();

        if (current == toid) {
            return current_path; //destination
        }

        // over all connections
        for (const auto& [conn_id, connection] : connections) {
            BiteID next;
            std::vector<Coord> conn_path = connection.path;

            if (connection.start == current) {
                next = connection.end;
            } else if (connection.end == current) {
                next = connection.start;
                std::reverse(conn_path.begin(), conn_path.end());
            } else {
                continue;
            }

            //  if the next BiteID is already visited
            if (visited.find(next) != visited.end()) {
                continue;
            }

            visited.insert(next);

            // New path and distance to the next BiteID
            std::vector<std::pair<Coord, Distance>> new_path = current_path;
            Coord last_coord = current_path.back().first;
            Distance accumulated_dist = current_dist;


            for (const Coord& c : conn_path) {
                Distance segment_dist = std::abs(c.x - last_coord.x) + std::abs(c.y - last_coord.y);
                accumulated_dist += segment_dist;
                new_path.emplace_back(c, accumulated_dist);
                last_coord = c;
            }


            Coord next_coord = bites[next].coord;
            Distance final_segment = std::abs(next_coord.x - last_coord.x) + std::abs(next_coord.y - last_coord.y);
            accumulated_dist += final_segment;
            new_path.emplace_back(next_coord, accumulated_dist);

            // Add next node to the queue
            q.push({ next, accumulated_dist, new_path });
        }
    }


    return {};

}


// Non-compulsory operations
std::vector<std::pair<Coord, Distance>> Datastructures::path_shortest(BiteID fromid,
                                                                      BiteID toid)
{
    // Check if both BiteIDs exist
    if (bites.find(fromid) == bites.end() || bites.find(toid) == bites.end()) {
        return { {NO_COORD, NO_DISTANCE} };
    }

    // If fromid and toid are the same
    if (fromid == toid) {
        return {};
    }

    // Priority queue for Dijkstra's
    using PQElement = std::tuple<Distance, BiteID, std::vector<std::pair<Coord, Distance>>>;
    std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>> pq;

    // Distance map and visited set
    std::unordered_map<BiteID, Distance> dist;
    std::unordered_set<BiteID> visited;

    // Initialize
    pq.push({0, fromid, { {bites[fromid].coord, 0} }});
    dist[fromid] = 0;

    while (!pq.empty()) {
        auto [current_dist, current_bite, current_path] = pq.top();
        pq.pop();

        if (current_bite == toid) {
            return current_path;
        }


        if (visited.find(current_bite) != visited.end()) continue;
        visited.insert(current_bite);

        // Iterate on all connections
        for (const auto& [conn_id, connection] : connections) {
            BiteID next;
            std::vector<Coord> conn_path = connection.path;

            if (connection.start == current_bite) {
                next = connection.end;
            } else if (connection.end == current_bite) {
                next = connection.start;
                std::reverse(conn_path.begin(), conn_path.end());
            } else {
                continue;
            }

            // Calculate distance to the next node
            Coord last_coord = current_path.back().first;
            Distance new_dist = current_dist;
            std::vector<std::pair<Coord, Distance>> new_path = current_path;

            for (const Coord& c : conn_path) {
                new_dist += std::abs(c.x - last_coord.x) + std::abs(c.y - last_coord.y);
                new_path.emplace_back(c, new_dist);
                last_coord = c;
            }

            // Move to the next BiteID
            Coord next_coord = bites[next].coord;
            Distance final_segment = std::abs(next_coord.x - last_coord.x) + std::abs(next_coord.y - last_coord.y);
            new_dist += final_segment;
            new_path.emplace_back(next_coord, new_dist);

            // If a shorter path found
            if (dist.find(next) == dist.end() || new_dist < dist[next]) {
                dist[next] = new_dist;
                pq.push({new_dist, next, new_path});
            }
        }
    }

    return {};

}
std::vector<std::pair<Coord, Distance>> Datastructures::path_least_bites(BiteID fromid,
                                                                         BiteID toid)
{
    // Check if both BiteIDs exist
    if (bites.find(fromid) == bites.end() || bites.find(toid) == bites.end()) {
        return { {NO_COORD, NO_DISTANCE} };
    }

    // If fromid and toid are the same
    if (fromid == toid) {
        return {};
    }

    // BFS queue
    std::queue<std::tuple<BiteID, Distance, std::vector<std::pair<Coord, Distance>>>> q;
    std::unordered_set<BiteID> visited;

    // Initialize the queue
    q.push({fromid, 0, { {bites[fromid].coord, 0} }});
    visited.insert(fromid);

    while (!q.empty()) {
        auto [current_bite, current_dist, current_path] = q.front();
        q.pop();

        if (current_bite == toid) {
            return current_path;
        }

        // Iterate on all connections
        for (const auto& [conn_id, connection] : connections) {
            BiteID next;
            std::vector<Coord> conn_path = connection.path;

            if (connection.start == current_bite) {
                next = connection.end;
            } else if (connection.end == current_bite) {
                next = connection.start;
                std::reverse(conn_path.begin(), conn_path.end());
            } else {
                continue;
            }


            if (visited.find(next) != visited.end()) continue;

            visited.insert(next);


            std::vector<std::pair<Coord, Distance>> new_path = current_path;
            Coord last_coord = current_path.back().first;
            Distance new_dist = current_dist;

            for (const Coord& c : conn_path) {
                new_dist += std::abs(c.x - last_coord.x) + std::abs(c.y - last_coord.y);
                new_path.emplace_back(c, new_dist);
                last_coord = c;
            }

            Coord next_coord = bites[next].coord;
            Distance final_segment = std::abs(next_coord.x - last_coord.x) + std::abs(next_coord.y - last_coord.y);
            new_dist += final_segment;
            new_path.emplace_back(next_coord, new_dist);

            q.push({next, new_dist, new_path});
        }
    }


    return {};

}
bool Datastructures::is_within_contour(Coord coord, const Contour& contour) {
    for (const auto& point : contour.coords) {
        if (point == coord) {
            return true;  // Coordinate found within the contour
        }
    }
    return false;  // Coordinate not found
}

ContourHeight Datastructures::retrieve_contour_height(Coord coord) {
    for (const auto& [bite_id, contour] : contours) {
        if (is_within_contour(coord, contour)) {
            return contour.height;  // Return the height
        }
    }
    return NO_CONTOUR_HEIGHT;
}

std::vector<std::pair<Coord, Distance>> Datastructures::path_least_uphill(BiteID fromid,
                                                                          BiteID toid)
{

        if (bites.find(fromid) == bites.end() || bites.find(toid) == bites.end()) {
            return { {NO_COORD, NO_DISTANCE} };
        }


        if (fromid == toid) {
            return {};
        }

        // Priority queue
        using PQElement = std::tuple<int, Distance, BiteID, std::vector<std::pair<Coord, Distance>>>;
        std::priority_queue<PQElement, std::vector<PQElement>, std::greater<>> pq;

        // Distance map and visited set
        std::unordered_map<BiteID, std::pair<Distance, int>> dist;
        std::unordered_set<BiteID> visited;

        // Initialize
        pq.push({0, 0, fromid, { {bites[fromid].coord, 0} }});
        dist[fromid] = {0, 0};

        while (!pq.empty()) {
            auto [current_hills, current_dist, current_bite, current_path] = pq.top();
            pq.pop();

            if (current_bite == toid) {
                return current_path;
            }

            if (visited.find(current_bite) != visited.end()) continue;
            visited.insert(current_bite);

            // Iterate on all connections
            for (const auto& [conn_id, connection] : connections) {
                BiteID next;
                std::vector<Coord> conn_path = connection.path;

                if (connection.start == current_bite) {
                    next = connection.end;
                } else if (connection.end == current_bite) {
                    next = connection.start;
                    std::reverse(conn_path.begin(), conn_path.end());
                } else {
                    continue;
                }


                Coord last_coord = current_path.back().first;
                Distance new_dist = current_dist;
                int new_hills = current_hills;
                std::vector<std::pair<Coord, Distance>> new_path = current_path;

                for (const Coord& c : conn_path) {
                    // Get the contour height
                    ContourHeight height = retrieve_contour_height(c);
                    Distance segment_distance = std::abs(c.x - last_coord.x) + std::abs(c.y - last_coord.y);


                    new_dist += segment_distance;

                    if (height > 0) {
                        new_hills += 1;

                    } else if (height < 0) {
                        new_hills += std::abs(height);

                    }


                    new_path.emplace_back(c, new_dist);
                    last_coord = c;  // Update last coordinate
                }

                // Move to the next BiteID
                Coord next_coord = bites[next].coord;
                Distance final_segment = std::abs(next_coord.x - last_coord.x) + std::abs(next_coord.y - last_coord.y);
                new_dist += final_segment;
                new_path.emplace_back(next_coord, new_dist);  // Update the path

                // If a shorter path is found or fewer hills encountered
                if (dist.find(next) == dist.end() || new_hills < dist[next].second) {

                    dist[next] = {new_dist, new_hills};
                    pq.push({new_hills, new_dist, next, new_path});
                }
            }
        }
        return {};
}
