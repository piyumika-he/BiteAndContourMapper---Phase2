// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include "customtypes.hh"
#include <utility>
#include <vector>

// Add your own STL includes below this comment
#include <unordered_set>
#include <queue>
#include <list>
#include <map>
#include<algorithm>
#include<functional>

template <typename Type>
Type random_in_range(Type start, Type end);

// This is the class you are supposed to implement

class Datastructures
{
public:
  Datastructures();
  ~Datastructures();

  // Estimate of performance: O(1)
  // Short rationale for estimate:Bites store in a map. returning the size of the map is
  // constant time operation.
  unsigned int get_bite_count();

  // Estimate of performance: O(n)
  // Short rationale for estimate: Bites and contours store in a map.
  // Clearing them is a linear time operation.
  void clear_all();

  // Estimate of performance: O(n)
  // Short rationale for estimate: Copying elements from a vector takes O(n) compexity
  std::vector<BiteID> all_bites();

  // Estimate of performance: O(n)
  // Short rationale for estimate: Adding and inserting into a map is O(logn) time complexity operation
  //distance_sorted_bites.insert(dist_it, id) makes an insertion into a vector, so it takes O(n).
  bool add_bite(BiteID id, const Name & name, Coord xy);

  // Estimate of performance: O(log n)
  // Short rationale for estimate: In maps find algorithm has O(log n) complexity
  Name get_bite_name(BiteID id);

  // Estimate of performance: O(log n)
  // Short rationale for estimate: In maps find algorithm has O(log n) complexity
  Coord get_bite_coord(BiteID id);

  // We recommend you implement the operations below only after implementing the
  // ones above

  // Estimate of performance: O(n)
  // Short rationale for estimate: This function returns a copy of the entire vector. Hence O(n)
  std::vector<BiteID> get_bites_alphabetically();

  // Estimate of performance: O(n)
  // Short rationale for estimate: This function returns a copy of the entire vector. Hence O(n)
  std::vector<BiteID> get_bites_distance_increasing();

  // Estimate of performance: O(log n)
  // Short rationale for estimate: searching a coordinate in the map takes O(log n).
  BiteID find_bite_with_coord(Coord xy);


  // Estimate of performance: O(log n)
  // Short rationale for estimate: Adding to map is logrithmic time operation on average
  bool add_contour(ContourID id, const Name & name, ContourHeight height,
                   std::vector<Coord> coords);

  // Estimate of performance: O(n)
  // Short rationale for estimate: Since the function iterates through the map it's linear time
  std::vector<ContourID> all_contours();

  // Estimate of performance: O(logn)
  // Short rationale for estimate: Hence seach in a map is the dominating factor here.
  Name get_contour_name(ContourID id);

  // Estimate of performance: O(n)
  // Short rationale for estimate: Coping coordinates has a linear time complexity.
  std::vector<Coord> get_contour_coords(ContourID id);

  // Estimate of performance: O(logn)
  // Short rationale for estimate: search for the std::map for the contour height
  ContourHeight get_contour_height(ContourID id);

  // Estimate of performance: O(logn)
  // Short rationale for estimate: search for the parent and child contours in the map take O(logn) complexity
  bool add_subcontour_to_contour(ContourID id, ContourID parentid);

  // Estimate of performance: O(logn)
  // Short rationale for estimate:Search for the contour and bite both takes logarithmic time complexity.
  bool add_bite_to_contour(BiteID biteid, ContourID parentid);

  // Estimate of performance: O(n)
  // Short rationale for estimate: Since there's a while loop which iterates through the contours
  std::vector<ContourID> get_bite_in_contours(BiteID id);

  // Non-compulsory operations

  // Estimate of performance: O(n)
  // Short rationale for estimate: Because of the recursive function
  std::vector<ContourID> all_subcontours_of_contour(ContourID id);

  // Estimate of performance: O(nlogn)
  // Short rationale for estimate:
  std::vector<BiteID> get_bites_closest_to(Coord xy);

  // Estimate of performance: O(logn)
  // Short rationale for estimate:
  bool remove_bite(BiteID id);

  // Estimate of performance: O(n)
  // Short rationale for estimate:
  ContourID get_closest_common_ancestor_of_contours(ContourID id1,
                                                    ContourID id2);




 /**
   * ============================
   * ============================
   * ========== PRG2 ============
   * ============================
   * ============================
   */

  // Estimate of performance: O(n)
  // Short rationale for estimate: since it iterates through the coordinates once to check for diagonal movement.
  bool add_connection(ConnectionID connectionid, BiteID id1, BiteID id2,
                      std::vector<Coord> xy);

  // Estimate of performance: O(n)
  // Short rationale for estimate: clear() function removes all elements from the unordered map, which involves deallocating memory for each entry.
  void clear_connections();

  // Estimate of performance: O(n)
  // Short rationale for estimate: As it iterates through all connections to find those associated with the given id
  std::vector<BiteID> get_next_bites_from(BiteID id);

  // Estimate of performance: O(n)
  // Short rationale for estimate: The function iterates through all connections to find those matching
  //the specified conditions based on the provided id1 and id2
  std::vector<ConnectionID> get_connections(BiteID id1 = NO_BITE, BiteID id2 = NO_BITE);

  // Estimate of performance: O(n)
  // Short rationale for estimate:  it concatenates the coordinates from the connection's path, resulting in linear complexity
  std::vector<Coord> get_connection_coords(BiteID biteid,
                                           ConnectionID connectionid);

  // Estimate of performance: O(n^2)
  // Short rationale for estimate: BFS loop iterates up to n times, and for each iteration checks connections and for each connection iterate through cordinates
  std::vector<std::pair<Coord, Distance>> path_any(BiteID fromid, BiteID toid);

  // Non-compulsory operations

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_shortest(BiteID fromid,
                                                        BiteID toid);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_least_bites(BiteID fromid,
                                                           BiteID toid);

  // Estimate of performance:
  // Short rationale for estimate:
  std::vector<std::pair<Coord, Distance>> path_least_uphill(BiteID fromid,
                                                            BiteID toid);






private:
  // Add stuff needed for your class implementation here

  struct Bite {
      Name name;
      Coord coord;
      ContourID contour_id;

  };

  std::map<BiteID, Bite> bites; // Map from BiteID to (Name, Coord)

  std::unordered_map<Coord, BiteID> coord_to_bite; // For faster search

  std::vector<BiteID> bite_ids;

  std::vector<BiteID> alphabetical_bites;

  std::vector<BiteID> distance_sorted_bites;



  struct Contour {
      Name name;
      ContourHeight height;
      std::vector<Coord> coords;
      ContourID parent_id = NO_CONTOUR; // ID of the parent contour
      std::vector<ContourID> sub_contours; // List of sub-contour IDs
  };
  std::map<ContourID, Contour> contours;

  void collect_subcontours(ContourID contour_id, std::vector<ContourID>& collected); // Declaration of the function

  struct Connection {
      BiteID start;
      BiteID end;
      std::vector<Coord> path;
  };

  // Data structure to store connections
  std::unordered_map<ConnectionID, Connection> connections;

  inline bool is_diagonal(const Coord& c1, const Coord& c2) {
      return (c1.x != c2.x && c1.y != c2.y); // Diagonal if both x and y change
  }

  ContourHeight retrieve_contour_height(Coord coord);  // Renamed function
  bool is_within_contour(Coord coord, const Contour& contour);


};



#endif // DATASTRUCTURES_HH
