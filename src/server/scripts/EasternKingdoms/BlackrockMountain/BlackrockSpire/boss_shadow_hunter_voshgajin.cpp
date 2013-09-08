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
#include "blackrock_spire.h"

enum Spells
{
    SPELL_CURSEOFBLOOD              = 24673,
    SPELL_HEX                       = 16708,
    SPELL_CLEAVE                    = 20691,
};

enum Events
{
    EVENT_CURSE_OF_BLOOD            = 1,
    EVENT_HEX                       = 2,
    EVENT_CLEAVE                    = 3,
};

class boss_shadow_hunter_voshgajin : public CreatureScript
{
public:
    boss_shadow_hunter_voshgajin() : CreatureScript("boss_shadow_hunter_voshgajin") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_shadowvoshAI(creature);
    }

    struct boss_shadowvoshAI : public BossAI
    {
        boss_shadowvoshAI(Creature* creature) : BossAI(creature, DATA_SHADOW_HUNTER_VOSHGAJIN) {}

        void Reset() OVERRIDE
        {
            _Reset();
            //DoCast(me, SPELL_ICEARMOR, true);
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 2 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_HEX,     8 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CLEAVE, 14 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_CURSE_OF_BLOOD:
                        DoCastVictim(SPELL_CURSEOFBLOOD);
                        events.ScheduleEvent(EVENT_CURSE_OF_BLOOD, 45 * IN_MILLISECONDS);
                        break;
                    case EVENT_HEX:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                            DoCast(target, SPELL_HEX);
                        events.ScheduleEvent(EVENT_HEX, 15 * IN_MILLISECONDS);
                        break;
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 7 * IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };

};

void AddSC_boss_shadowvosh()
{
    new boss_shadow_hunter_voshgajin();
}
