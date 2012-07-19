#include "SAP.h"
#include "CollisionManager.h"
#include <iostream>

// Sweep and Prune implementation
// Adapted from Art Of Collision tutorial
// http://www.shmup-dev.com/forum/index.php?page=24

namespace Amju
{
#define DESCRIBE(go) go->GetId() << " (" << go->GetTypeName() << ")"

SweepAndPrune::SweepAndPrune()
{
    m_collideFunc = 0;
    Clear();
}

void SweepAndPrune::Clear()
{
    m_numBoxes = 0;
    m_encounters.clear();
}

void SweepAndPrune::SetCollideFunc(CollideFunc cf)
{
    m_collideFunc = cf;
}

void SweepAndPrune::Update(const GameObjects& gos)
{
    if (m_numBoxes == 0)
    { 
        for (GameObjects::const_iterator it = gos.begin(); it != gos.end(); ++it)
        {
            GameObject* go = it->second;
            AddBox(go);
        }
        // Initial sort
        Update();
std::cout << "******** SAP finished adding and sorting **********\n";
        return;
    }

    Update();
    ResolveEncounters();
}

void SweepAndPrune::Update()
{
    // sort lists on each axis
    for (int axis = 0; axis < AMJU_SAP_AXES; axis++)
    {
        // go through each endpoint in turn
        for (int j = 1; j < m_numBoxes*2; j++)
        {
            Endpoint::Type keyType = m_endpoints[j][axis].m_type;
            GameObject* keyGO = m_endpoints[j][axis].m_go;
            Assert(keyGO);

            const AABB* aabb = keyGO->GetAABB();
            if (!aabb)
            {
                continue;
            }
            float keyVal = (keyType == Endpoint::AMJU_SAP_MIN) ? aabb->GetMin(axis) : aabb->GetMax(axis);

            // Compare the keyval to the value one before it in the array (our comparison value) 
            //and swap places if need be. 
            // Keep doing this until no more swaps are needed or until we reach the start of the array
            int i = j-1;
            while (i >= 0)
            {
                // get our comparison value in the same way we got the key value
                Endpoint::Type compType = m_endpoints[i][axis].m_type;
                GameObject* compGO = m_endpoints[i][axis].m_go;
                Assert(compGO);

                const AABB* aabb = compGO->GetAABB();
                if (!aabb)
                {
                    i--; // ho ho
                    continue;
                }
                float compVal = (compType == Endpoint::AMJU_SAP_MIN) ? aabb->GetMin(axis) : aabb->GetMax(axis);

                if (compVal <= keyVal)
                {
                    // these values are in the correct order so break out of this while loop
                    break; 
                }

                // these vals are swapping places which relates to a change in the state of the scene
                // so update our encounter list accordingly

                // if and endpoint is swapping to the left on a startpoint then we know 
                // these objects are leaving contact
                // so remove any encounters relating to these objects
                if ((compType == Endpoint::AMJU_SAP_MIN) && (keyType == Endpoint::AMJU_SAP_MAX))
                {
                    // NB Order sawapped compared to Add, does this mean we don't need to check both ?
                    RemoveEncounter(compGO, keyGO); 

std::cout << "SAP: Objs " << DESCRIBE(compGO) << " and " << DESCRIBE(keyGO) << " detached in axis " << axis << "\n";

                }
                else if((compType == Endpoint::AMJU_SAP_MAX) && (keyType == Endpoint::AMJU_SAP_MIN))
                {
                    // else if a startpoint is swapping to the left of an end point, these object might be colliding
                    // this only tells us that they overlap on one axis
                    // to be sure of collision we must do an intersection test
                    if (m_collideFunc(keyGO, compGO)) 
                    {
                        AddEncounter(keyGO, compGO); 

std::cout << "SAP: Objs " << DESCRIBE(compGO) << " and " << DESCRIBE(keyGO) 
  << " COLLIDED! (Swapped axis " << axis << ")\n";

                    }
                    else 
                    {
std::cout << "SAP: Objs " << DESCRIBE(compGO) << " and " << DESCRIBE(keyGO) 
  << " swapped in axis " << axis << " but no collision\n";
 
                    }
                }

                // finaly we must swap the points
                m_endpoints[i+1][axis].m_type = compType;
                m_endpoints[i][axis].m_type = keyType;

                m_endpoints[i+1][axis].m_go = compGO; 
                m_endpoints[i][axis].m_go = keyGO; 

                // we must decriment i so that we continue searching down the array
                i--;
            }
        }
    }
}

void SweepAndPrune::ResolveEncounters()
{
    // Iterate through your encounter list and trigger collision resolution code
    // for each pair of objects in there

    // Copy the container so iterator won't be invalidated
    EncounterSet encs = m_encounters;
    for (EncounterSet::iterator it = encs.begin(); it != encs.end(); ++it)
    {
        const Encounter& enc = *it;
        bool b = TheCollisionManager::Instance()->HandleCollision(enc.first, enc.second);
        if (b)
        {
std::cout << "SAP: Handled collision between " << DESCRIBE(enc.first) << " and " << DESCRIBE(enc.second) << "\n";
        
            // TODO if reqired by game, remove encounter now
        }
        else
        {
std::cout << "SAP: NO HANDLER for collision between " << DESCRIBE(enc.first) << " and " << DESCRIBE(enc.second) << "\n";

            RemoveEncounter(enc.first, enc.second); // may as well remove it
        }
    }
}

void SweepAndPrune::AddEncounter(GameObject* go1, GameObject* go2)
{
    // Add encounter between the inputted objects to the list
    // being careful not to duplicate existing ecounters

    if (m_encounters.find(std::make_pair(go1, go2)) == m_encounters.end() &&
        m_encounters.find(std::make_pair(go2, go1)) == m_encounters.end())
    {
std::cout << "SAP: Adding encounter between " << DESCRIBE(go1) " and " << DESCRIBE(go2) << "\n";

        m_encounters.insert(std::make_pair(go1, go2));
    }
}

void SweepAndPrune::RemoveEncounter(GameObject* go1, GameObject* go2) ////int objIdA, int objIdB)
{
    // Remove encounter between the inputted objects from the
    // list if it exists
    EncounterSet::iterator it;
    it = m_encounters.find(std::make_pair(go1, go2));
    if (it != m_encounters.end())
    {
std::cout << "SAP: Removing encounter between " << DESCRIBE(go1) " and " << DESCRIBE(go2) << "\n";

        m_encounters.erase(it);
        return;
    }
    it = m_encounters.find(std::make_pair(go2, go1));
    if (it != m_encounters.end())
    {
std::cout << "SAP: Removing encounter between " << DESCRIBE(go1) " and " << DESCRIBE(go2) << "\n";

        m_encounters.erase(it);
        return;
    }
    // Could be called during first sorting pass
}

int SweepAndPrune::AddBox(GameObject* go) 
{
std::cout << "SAP: Adding box for obj " << DESCRIBE(go) << "\n";

    // add endpoints
    int nb2 = 2 * m_numBoxes;
    for (int i = 0; i < AMJU_SAP_AXES; i++)
    {
        m_endpoints[nb2][i].m_type = Endpoint::AMJU_SAP_MIN;
        m_endpoints[nb2][i].m_go = go; 
        m_endpoints[nb2 + 1][i].m_type = Endpoint::AMJU_SAP_MAX;
        m_endpoints[nb2 + 1][i].m_go = go; 
    }

    // note that its not important to insert these in their correct sorted value position
    // as I havn't since the next call to Update() will sort that out for us.

    m_numBoxes++;

    // return the id of this box so that objects know which box to update when they move
    return m_numBoxes - 1;
}

/*
void SweepAndPrune::RemoveBox(GameObject* go)
{
    // Remove the box with the corresponding id being careful to remove all its
    // endpoints as well as any encounters relating to this boxes object
}
*/
} // namespace 


