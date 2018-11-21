Kattis and UVa MaxFlow and Graph Matching problems
==================================================

## Internet Bandwidth

## Clever Naming Pattern

### Type: Two Layers of Bipartite Matching

**Idea**: For each instance, note the first letter used for names for each problem. Link source to each letter with capacity 1, then each letter to the problem where it appears and each problem to the given word for it.

## The Programmers

## Crimewave

## ✅ March of the Penguins

### Type: Iterated Decision problem using Max Flow

**Idea**: represents blocks as vertices with capacity for the number of jumps that can be performed. Link source to blocks with the number of penguins as capacities. Compute distance between blocks and link them with infinite capacity if jumping from one to the other is possible. Then perform `n` MaxFlow on each block as target to know if penguins can reach this one. Save and then print the one that are reachable.


## 🤔 Avoiding the Apocalypse

## ✅ Transportation Deleguation

### Type: Max flow problem

**Idea**: unit vertex capacities on the transportation company. Link each tranport company to the states where they can operate both way. Link source to rawsite, like factories to sink. Run Maxflow.


### Type:


## Duopoly

## Buy one, get the rest free



**Idea**

## Risk

## Power Transmission

## ✅ Councilling

## Data flow

## ✅ Down Went the Titanic

**Idea** : see tutorial

## It's all about the Bandwidth

## Coyst

## ✅ It can be arranged

### Type: Min-Path-Cover solvable with Max Flow

**Idea:** For each course, compute the number of rooms needed (`ceil(nb_students/M)`). Then construct a two layers graphs: the first layer represent course that can rooms, the second courses that can get rooms. Connect source to the first layer with the number of rooms as edges capacities. Connect the source. Connect `i` in first layer to `j` with inf capacity if course `i` ends and can be cleaned before `j` begins.
The answer is the total number of rooms minus the max flow


## Piano

## ✅ Tomography

### Type: Decision problem using MaxFlow

**Idea**: Constructs two layers graphs, row sums as first layer vertices (edge capacity as those sums), ditto for columns sum with second columns. Fully connected the two layers with capacity one. Each link represents the degree left to "light" a case in the table or not. If the MaxFlow equals the sum then it is possible to find a configuration (`Yes`) else it's not (`No`).

## Waif

### Type: Max flow

**Idea** : maxflow, children on the left, toys in the middle, categories on the right. Link source to children, children to toys, toys to their categ with unit cap. Categ to target with their capacity. Toys that do not belongs to any category are directly linked to the target.

## Nuts and Bolts

## Attacking rooks

## Bookclub

## Elementary Math

## ✅ Escape Plan

### Type: Iterated MCBM

**Idea**: Build graph if robot i can reach hole j in time. Add new edge for each new time incrementally and just reset the graph. Rerun max flow every time to have answer for each time.

## Bileteral

## ✅ Taxi Cab Scheme

### Type: Min-Path Cover (similar to "It can be arranged")

**Idea**: Two layers graphs with unit capaicty everywhere and with nodes denoting rides. Link i in first layers to j in second layer if one taxi can perform ride j after ride i (use distance and time to judge). Run max flow ; the answer is the number of rides minus the value of the max flow.


## Against Mammoths

### Type: Iterated Max Flow

**Idea**:

Criteria of choice of attack at time `t`:

n_ship_init - n_mammoth_init - time_to_travel_there * prod_rate_mammoth + (prod_rate_ship - prod_rate_mammoth) * t ≥ 0

If, for one planet (prod_rate_ship - prod_rate_mammoth) < 0 or (prod_rate_ship - prod_rate_mammoth) == 0 and the rest is negative then it is impossible.

Otherwise augment t, and at each time perform matching incrementally.

Victory times is when max flow == number of ennemies planets.

## Sorting Slides

## The dog task

**Idea**: see tutorial

## A plug for UNIX

## Type: MaxFlow

**Idea**: connect devices that can directly be connected to plugs. Link remaining devices to adaptors, adaptors to plugs, and adaptor to each other. Unit capacity on adaptor. Run max flow. The answer is the number of devices mimus the max flow.

### Type:  Similar to transportation problem

## Ware House

### Type: ASSP with Maxflow

**Idea**: for each block compute the path length to the placeholders. Construct bipartite graph with boxes on the left, placeholders on the right with distance negated as edge capacities. Then run Min Weighted Bipartite matching to match to closes. Re-negated if needed.
