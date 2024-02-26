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

### Entry #5 [Feb 26th, 2024]:
- Catacomb asset texturing to world applied
- Enable multiple tile layers in world file to stack correctly.
- Enable multiple tile sets across multiple tile layers to render as expected.
- Guard pathing updated to patrol around catacombs.
- Update player spawn location(s)
- Update lighting approach to use polygons rather than rects. 
This greatly improves performance from the reduction in vertices. 
- Update collision logic to be based on polygons and polylines rather than at the tile level.
- Add end zone to the stage that the player must reach to complete the stage.
- Add overlays to darken map and a lighter ellipse to give more visibility around player. This
is done through blending ([multiply] and [add] respectively).

##### First Stage Spawn Area
![Mon 26 Feb 2024 18:16:51 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/4b636cd1-5003-4e73-8271-0ed7708ef53a)

##### Lighting showcased
![Mon 26 Feb 2024 18:17:34 GMT](https://github.com/Ticketedmoon/Crucible/assets/21260839/ed475168-2579-4647-b8f1-b63830321d07)


---

### Credit
- https://ncase.me/sight-and-light/
