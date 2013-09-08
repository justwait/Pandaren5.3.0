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
    SPELL_BLASTWAVE                 = 11130,
    SPELL_SHOUT                     = 23511,
    SPELL_CLEAVE                    = 20691,
    SPELL_KNOCKAWAY                 = 20686
};

enum Events
{
    EVENT_BLAST_WAVE                = 1,
    EVENT_SHOUT                     = 2,
    EVENT_CLEAVE                    = 3,
    EVENT_KNOCK_AWAY                = 4
};

enum Adds
{
    NPC_SPIRESTONE_WARLORD          = 9216,
    NPC_SMOLDERTHORN_BERSERKER      = 9268
};

const Position SummonLocation1 = { -39.355f, -513.456f, 88.472f, 4.679f };
const Position SummonLocation2 = { -49.875f, -511.896f, 88.195f, 4.613f };

class boss_overlord_wyrmthalak : public CreatureScript
{
public:
    boss_overlord_wyrmthalak() : CreatureScript("boss_overlord_wyrmthalak") { }

    CreatureAI* GetAI(Creature* creature) const OVERRIDE
    {
        return new boss_overlordwyrmthalakAI(creature);
    }

    struct boss_overlordwyrmthalakAI : public BossAI
    {
        boss_overlordwyrmthalakAI(Creature* creature) : BossAI(creature, DATA_OVERLORD_WYRMTHALAK) {}

        bool Summoned;

        void Reset() OVERRIDE
        {
            _Reset();
            Summoned = false;
        }

        void EnterCombat(Unit* /*who*/) OVERRIDE
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_BLAST_WAVE, 20 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_SHOUT,       2 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_CLEAVE,      6 * IN_MILLISECONDS);
            events.ScheduleEvent(EVENT_KNOCK_AWAY, 12 * IN_MILLISECONDS);
        }

        void JustDied(Unit* /*killer*/) OVERRIDE
        {
            _JustDied();
        }

        void UpdateAI(uint32 diff) OVERRIDE
        {
            if (!UpdateVictim())
                return;

            if (!Summoned && HealthBelowPct(51))
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                {
                    if (Creature* warlord = me->SummonCreature(NPC_SPIRESTONE_WARLORD, SummonLocation1, TEMPSUMMON_TIMED_DESPAWN, 300 * IN_MILLISECONDS))
                        warlord->AI()->AttackStart(target);
                    if (Creature* berserker = me->SummonCreature(NPC_SMOLDERTHORN_BERSERKER, SummonLocation2, TEMPSUMMON_TIMED_DESPAWN, 300 * IN_MILLISECONDS))
                        berserker->AI()->AttackStart(target);
                    Summoned = true;
                }
            }

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_BLAST_WAVE:
                        DoCastVictim(SPELL_BLASTWAVE);
                        events.ScheduleEvent(EVENT_BLAST_WAVE, 20 * IN_MILLISECONDS);
                        break;
                    case EVENT_SHOUT:
                        DoCastVictim(SPELL_SHOUT);
                        events.ScheduleEvent(EVENT_SHOUT, 10 * IN_MILLISECONDS);
                        break;
                    case EVENT_CLEAVE:
                        DoCastVictim(SPELL_CLEAVE);
                        events.ScheduleEvent(EVENT_CLEAVE, 7 * IN_MILLISECONDS);
                        break;
                    case EVENT_KNOCK_AWAY:
                        DoCastVictim(SPELL_KNOCKAWAY);
                        events.ScheduleEvent(EVENT_KNOCK_AWAY, 14 * IN_MILLISECONDS);
                        break;
                }
            }
            DoMeleeAttackIfReady();
        }
    };
};

void AddSC_boss_overlordwyrmthalak()
{
    new boss_overlord_wyrmthalak();
}
