/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2013 Project Cerberus <http://www.erabattle.ru/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is not free software; you can not redistribute it and/or modify it.
 *
 * This program is distributed only by <http://www.erabattle.ru/>!
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Spells
{
    SPELL_HEAL                                             = 10917,
    SPELL_RENEW                                            = 10929,
    SPELL_SHIELD                                           = 10901,
    SPELL_MINDBLAST                                        = 10947,
    SPELL_SHADOWWORDPAIN                                   = 10894,
    SPELL_SMITE                                            = 10934
};

class boss_moira_bronzebeard : public CreatureScript
{
public:
    boss_moira_bronzebeard() : CreatureScript("boss_moira_bronzebeard") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_moira_bronzebeardAI(creature);
    }

    struct boss_moira_bronzebeardAI : public ScriptedAI
    {
        boss_moira_bronzebeardAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 Heal_Timer;
        uint32 MindBlast_Timer;
        uint32 ShadowWordPain_Timer;
        uint32 Smite_Timer;

        void Reset() OVERRIDE
        {
            Heal_Timer = 12000;                                 //These times are probably wrong
            MindBlast_Timer = 16000;
            ShadowWordPain_Timer = 2000;
            Smite_Timer = 8000;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE {}

        void UpdateAI(uint32 diff) OVERRIDE
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            //MindBlast_Timer
            if (MindBlast_Timer <= diff)
            {
                DoCastVictim(SPELL_MINDBLAST);
                MindBlast_Timer = 14000;
            } else MindBlast_Timer -= diff;

            //ShadowWordPain_Timer
            if (ShadowWordPain_Timer <= diff)
            {
                DoCastVictim(SPELL_SHADOWWORDPAIN);
                ShadowWordPain_Timer = 18000;
            } else ShadowWordPain_Timer -= diff;

            //Smite_Timer
            if (Smite_Timer <= diff)
            {
                DoCastVictim(SPELL_SMITE);
                Smite_Timer = 10000;
            } else Smite_Timer -= diff;
        }
    };
};

void AddSC_boss_moira_bronzebeard()
{
    new boss_moira_bronzebeard();
}
