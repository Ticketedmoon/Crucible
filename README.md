# Crucible 

---

## What is this project?
A couch Co-Op 2D top-down stealth rogue-like game.

---

## Tools
- C++
- SFML
- CMake

---

## Dev Log

### Entry #1 [Nov 14th -> Nov 26th, 2023]:
- Basic 2D geometry added to the world.
- Basic player controls.
- Ability to push and move objects in the world.
- Introduction of entity memory pooling.

I did not take screenshots during this period.

---

### Entry #2 [Nov 27th -> Dec 30th, 2023]:
Not a huge amount of time was spent working on the project during this period due to work and with the Christmas holidays.
However, I did start a separate branch on the project for building my own line segment intersection algorithm.

The goal with this was to add a visibility system to the game so we could determine what is visible to a given entity.

This went through many changes as a result of the wrong strategy being used. The initial implementation worked for
a set of rays in a radial pattern around the player entity. However, when joining these rays with a TriangleFan there 
was a lot of jagged edges, flickering and clipping over the corners of other world entities. Eventually, I managed to
succeed in getting this to work. 

Here are the changes summarised during this period:
- Visibility System with Line Segment Intersection

**Screenshots**
![Sat 30 Dec 2023 16:50:20 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/1ef528d2-6c34-4128-b85f-1549b1c84021)
![Sat 30 Dec 2023 16:57:20 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/4e6bc14b-58e1-4ffb-88fd-89ae573f7f57)

- The pinky block represents our player and light source. 
- The yellow area represents the region the player can see.

---

### Entry #3 [Jan 21st, 2024]:

Haven't had much time to work on this project over the last month, but except for one notable fix:
- Fixed a bug where the light rays would flicker when pushing entities in the game world.

---

### Entry #4 [Feb 1st, 2024]:

- Added tiling to the world with single tileset.
- Add spawn point (blue tile)
- Add end zone (green tiles)
- Maintain performant visibility system without ray casting to each tile vertex.
- Arrow blocks can be pushed
- Add visibility system blend (sf::Multiply) for the background tiles to still remain visible
while lit.

##### Add tiling to the world
![Mon 26 Feb 2024 18:48:17 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/04337ee5-fac8-45ae-a5ce-9e93b9a5a457)

##### Collision and Block-Pushing behaviour functional
![Mon 26 Feb 2024 18:48:34 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/20c7931a-69b6-4c14-ab79-083503d3db5e)


---

### Entry #5 [Feb 14th, 2024]:

- Added a new tileset to game world (purple-dungeon-brick)
- Add spawn tiles
- Add end tiles
- Add patrolling guards (No guard texture, just a patrolling brick texture)
- Merged with existing tileset from Entry #4 (See the spawn zone and end zone)
- Add lower level for player-2 for Couch Co-op. (An idea I think I want to toy with is
if one of the two player's is caught, the other player can additionally complete the
opposing section of the map to beat the stage.)

![Mon 26 Feb 2024 18:39:16 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/64d03c22-69a0-43eb-8021-642c3ab65823)

---

### Credit
- https://ncase.me/sight-and-light/
