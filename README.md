Kattis and UVa MaxFlow and Graph Matching problems
==================================================

------------------------

## ✅ Bookclub

### Type: Decision Problem on Perfect MCBM

**Idea**: Trivial

------------------------

## ✅ Internet Bandwidth
**Idea**: Trivial problem if applying max flow, simply build graph as described and run max flow on it

------------------------

## ✅ Clever Naming Pattern

### Type: Two Layers of Bipartite Matching

**Idea**: For each instance, note the first letter used for names for each problem. Link source to each letter with capacity 1, then each letter to the problem where it appears and each problem to the given word for it.
  - beware not to add flow to letters that are outside of the scope
  - Save all words for each problem in vector
  - Extract first letter from flow graph
  - Find word in vector with that letter
  - Sort words and output

------------------------


## ✅ The Programmers

### Type: Simple Max Flow problem

**Idea**: Construct graph as follows:
  - For each programmer and site add vertex
  - Add edge from source to all programmers with capacity 1
  - Add edge from site to sink with capacity C
  - Add edge between given (programmer, site) with capacity 1337/42/any other cool number
  - Print max flow

------------------------

## ✅ March of the Penguins

### Type: Iterated Decision problem using Max Flow

**Idea**: represents blocks as vertices with capacity for the number of jumps that can be performed. Link source to blocks with the number of penguins as capacities. Compute distance between blocks and link them with infinite capacity if jumping from one to the other is possible. Then perform `n` MaxFlow on each block as target to know if penguins can reach this one. Save and then print the one that are reachable.

------------------------

## ✅   Avoiding the Apocalypse

**Idea**: Model the problem like the following:
- split each node in the graph up into S levels
    - except for source node and medical facilities
- here level represents being able to get there within a certain time period.
- For each medical facility add edge to sink with infinite capacity
- From the source add edges to all neighbors at levels greater or equal to the time it takes to cross the road
    - except if endpoint is hospital, in which case we increase the max flow value by (s - t) * p
- Recursively for each neighbour in the previous step
    - add edge to next level with infinite capacity, and recurse on that node
    - add edge to neighbours if the current time + the time it takes to reach the node is less than threshold, and recurse on neighbor
- output max flow

------------------------

## ✅ Transportation Deleguation

### Type: Max flow problem

**Idea**: unit vertex capacities on the transportation company. Link each tranport company to the states where they can operate both way. Link source to rawsite, like factories to sink. Run Maxflow.


------------------------

## ✅ Down Went the Titanic

**Idea** : see tutorial

------------------------

## ✅ It can be arranged


### Type: Min-Path-Cover solvable with Max Flow

**Idea:** For each course, compute the number of rooms needed (`ceil(nb_students/M)`). Then construct a two layers graphs: the first layer represent course that can rooms, the second courses that can get rooms. Connect source to the first layer with the number of rooms as edges capacities. Connect the source. Connect `i` in first layer to `j` with inf capacity if course `i` ends and can be cleaned before `j` begins.
The answer is the total number of rooms minus the max flow

------------------------

## ✅ Tomography

### Type: Decision problem using MaxFlow

**Idea**: Constructs two layers graphs, row sums as first layer vertices (edge capacity as those sums), ditto for columns sum with second columns. Fully connected the two layers with capacity one. Each link represents the degree left to "light" a case in the table or not. If the MaxFlow equals the sum then it is possible to find a configuration (`Yes`) else it's not (`No`).

------------------------

## ✅ Waif until dark

### Type: Max flow

**Idea** : maxflow, children on the left, toys in the middle, categories on the right. Link source to children, children to toys, toys to their categ with unit cap. Categ to target with their capacity. Toys that do not belongs to any category are directly linked to the target.

------------------------

## ✅ Escape Plan

### Type: Iterated MCBM

**Idea**: Build graph if robot i can reach hole j in time. Add new edge for each new time incrementally and just reset the graph. Rerun max flow every time to have answer for each time.

------------------------

## ✅ Taxi Cab Scheme

### Type: Min-Path Cover (similar to "It can be arranged")

**Idea**: Two layers graphs with unit capaicty everywhere and with nodes denoting rides. Link i in first layers to j in second layer if one taxi can perform ride j after ride i (use distance and time to judge). Run max flow ; the answer is the number of rides minus the value of the max flow.

------------------------

## Against Mammoths

### Type: Iterated Max Flow

**Idea**:

Criteria of choice of attack at time `t`:
```
n_ship_init - n_mammoth_init - time_to_travel_there * prod_rate_mammoth + (prod_rate_ship - prod_rate_mammoth) * t ≥ 0
```

If, for one planet `(prod_rate_ship - prod_rate_mammoth) < 0` or `(prod_rate_ship - prod_rate_mammoth) == 0` and the rest is negative then it is impossible. Otherwise augment `t`, and at each time perform matching incrementally.

Victory times is when `max flow == number of ennemies planets`.

------------------------

### Type: ASSP with Maxflow

**Idea**: for each block compute the path length to the placeholders. Construct bipartite graph with boxes on the left, placeholders on the right with distance negated as edge capacities. Then run Min Weighted Bipartite matching to match to closes. Re-negated if needed.

------------------------

## 11167 - Monkeys in the Emei Mountain

**Idea**: Decision problem can be solved using max flow with the following modelling:
- Add vertice for each monkey with flow from source to monkey of v
- For all overlapping intervals combine it into one meaning
    - Add outgoing flow from overlapping interval to sink with capacity m * (start-end)
    - For each monkey assosiated that can drink in the interval add edge from monkey to interval with capacity (start-end)
- Run max flow on the graph, it is solveable if max flow is the same as the sum of all monkey's thrist.

For outputting an assignment i have tried the following heuristic (failed - Wrong Answer):
- For each max flow assignment of monkeys to an interval
    - greedily allocate them a time slot to drink by taking the m monkey(s) that need to drink the most before the end of the interval

------------------------

## The dog task

**Idea**: see tutorial

------------------------

## A plug for UNIX

### Type:  Similar to transportation problem

**Idea**: connect devices that can directly be connected to plugs. Link remaining devices to adaptors, adaptors to plugs, and adaptor to each other. Unit capacity on adaptor. Run max flow. The answer is the number of devices minus the max flow.

------------------------

## Buy one, get the rest free

### Type: Iterated MaxFlow problem

**Idea**: Construct a base layer graph over time: one vertex per day and per city:
 - Connect node (day_i, city_j) to (day_{i+1}, city_j) with infinite capacity for all j.
 - Connect source to (day_0, city_j) for all j
 - The sink is the node (last_day, last_city)

Then binary search on the price, and add flight whose price is less than this one. Perform max flow for the decision.

At the end, check for feasability ; if it is feasible outputs the price else "Impossible".

------------------------

## ✅ Councilling

### Type: Simple Max Flow problem
**Idea**:
Construct graph with 3 layers: clubs, residents, parties:
 - link source to clubs with unit capacity
 - link clubs to residents with unit capacity based on their membership
 - link residents to parties with unit capacity based on their membership
 - link parties to sink with ceil((C-1)/2) as a capacity

Run MaxFlow and get the assignment.

------------------------

## Risk

------------------------

## Duopoly

------------------------

## Crimewave

------------------------

## Power Transmission

------------------------

## Data flow

------------------------

## It's all about the Bandwidth

------------------------

## Coyst

------------------------

## ✅ Piano

### Type: 2 phases Decision problem with Max flow

**Idea**: the problem can be broken down in two pieces:
 - is it possible to only work on week-days?
 - if not, is it then possible with working on week-ends as well

Hence, construct a graph with source to piano, piano to given week days
then days to source. If max flow == n then "fine".

Else add week-ends and if maxflow == n then "weekend work" else "serious trouble".

------------------------

## Nuts and Bolts

### Type : complete implementation of an algorithm

------------------------

## Attacking rooks

------------------------

## Elementary Math

------------------------

## Sorting Slides

### Type: Mix of decision problem with MawFlow

**Implementation to finish**

**Idea**:
Bipartite Graph between slides and numbers. Unit edge if number is in the slide.
If MCBM != N, not possible.
Save the matching.
Then try to find the unique isomorphism by removing one matched edge at the
time. If the MBCM does not decrease monotically then not possible.
Else print the isomorphism.

------------------------

## Ware House

------------------------


## Bileteral

------------------------
