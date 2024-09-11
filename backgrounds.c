// Character background system
// From D&D 5e and PF1e

#include "conf.h"
#include "sysdep.h"
#include "structs.h"
#include "utils.h"
#include "comm.h"
#include "spells.h"
#include "handler.h"
#include "db.h"
#include "interpreter.h"
#include "constants.h"
#include "backgrounds.h"
#include "treasure.h"
#include "handler.h"
#include "fight.h"

int background_sort_info[NUM_BACKGROUNDS];
struct background_data background_list[NUM_BACKGROUNDS];

int compare_backgrounds(const void *x, const void *y)
{
  int a = *(const int *)x,
      b = *(const int *)y;

  return strcmp(background_list[a].name, background_list[b].name);
}

/* sort backgrounds called at boot up */
void sort_backgrounds(void)
{
  int a;

  /* initialize array, avoiding reserved. */
  for (a = 1; a < NUM_BACKGROUNDS; a++)
    background_sort_info[a] = a;

  qsort(&background_sort_info[1], NUM_BACKGROUNDS, sizeof(int), compare_backgrounds);
}

static void backgroundo(int background, const char *name, int skill_one, int skill_two, int featnum, const char *desc)
{
    background_list[background].name = name;
    background_list[background].desc = desc;
    background_list[background].skills[0] = skill_one;
    background_list[background].skills[1] = skill_two;
    background_list[background].feat = featnum;
}

void initialize_background_list(void)
{
  int i;

  /* initialize the list of feats */
  for (i = 0; i < NUM_BACKGROUNDS; i++)
  {
    background_list[i].name = "Unused Background";
    background_list[i].desc = "Unused Background";
    background_list[i].skills[0] = 0;
    background_list[i].skills[1] = 0;
    background_list[i].feat = 0;
  }
}

void assign_backgrounds(void)
{
    initialize_background_list();

    backgroundo(BACKGROUND_ACOLYTE, "acolyte", ABILITY_SENSE_MOTIVE, ABILITY_RELIGION, FEAT_BG_ACOLYTE,
        "You have spent your life in the service of a temple to a specific god or pantheon of gods. "
        "You act as an intermediary between the realm of the holy and the mortal world, performing "
        "sacred rites and offering sacrifices in order to conduct worshipers into the presence of "
        "the divine. You are not necessarily a cleric—performing sacred rites is not the same thing "
        "as channeling divine power.");

    backgroundo(BACKGROUND_CHARLATAN, "charlatan", ABILITY_SLEIGHT_OF_HAND, ABILITY_BLUFF, FEAT_BG_CHARLATAN,
        "You have always had a way with people. You know what makes them tick, you can tease out their "
        "hearts’ desires after a few minutes of conversation, and with a few leading questions you can "
        "read them like they were children’s books. It’s a useful talent, and one that you’re perfectly "
        "willing to use for your advantage. You know what people want and you deliver, or rather, you "
        "promise to deliver. Common sense should steer people away from things that sound too good to "
        "be true, but common sense seems to be in short supply when you’re around. The bottle of pink-colored "
        "liquid will surely cure that unseemly rash, this ointment — nothing more than a bit of fat with a "
        "sprinkle of silver dust — can restore youth and vigor, and there’s a bridge in the city that just "
        "happens to be for sale. These marvels sound implausible, but you make them sound like the real deal.");

    backgroundo(BACKGROUND_CRIMINAL, "criminal", ABILITY_BLUFF, ABILITY_STEALTH, FEAT_BG_CRIMINAL,
        "You are an experienced criminal with a history of breaking the law. You have spent a lot of time "
        "among other criminals and still have contacts within the criminal underworld. You’re far closer "
        "than most people to the world of murder, theft, and violence that pervades the underbelly of "
        "civilization, and you have survived up to this point by flouting the rules and regulations of society.");

    backgroundo(BACKGROUND_ENTERTAINER, "entertainer", ABILITY_ACROBATICS, ABILITY_PERFORM, FEAT_BG_ENTERTAINER,
        "You thrive in front of an audience. You know how to entrance them, entertain them, and even inspire them. "
        "Your poetics can stir the hearts of those who hear you, awakening grief or joy, laughter or anger. Your "
        "music raises their spirits or captures their sorrow. Your dance steps captivate, your humor cuts to the "
        "quick. Whatever techniques you use, your art is your life.");

    backgroundo(BACKGROUND_FOLK_HERO, "folk hero", ABILITY_HANDLE_ANIMAL, ABILITY_SURVIVAL, FEAT_BG_FOLK_HERO,
        "You come from a humble social rank, but you are destined for so much more. Already the people of your "
        "home village regard you as their champion, and your destiny calls you to stand against the tyrants and "
        "monsters that threaten the common folk everywhere.");

    backgroundo(BACKGROUND_GLADIATOR, "gladiator", ABILITY_ACROBATICS, ABILITY_PERFORM, FEAT_BG_GLADIATOR,
        "A gladiator is as much an entertainer as any minstrel or circus performer, trained to make the arts "
        "of combat into a spectacle the crowd can enjoy. This kind of flashy combat is your entertainer routine, "
        "though you might also have some skills as a tumbler or actor.");

    backgroundo(BACKGROUND_TRADER, "trader", ABILITY_SENSE_MOTIVE, ABILITY_DIPLOMACY, FEAT_BG_TRADER,
        "You are a member of an artisan’s guild, skilled in a particular field and closely associated "
        "with other artisans. You are a well-established part of the mercantile world, freed by talent "
        "and wealth from the constraints of a feudal social order. You learned your skills as an apprentice "
        "to a master artisan, under the sponsorship of your guild, until you became a master in your own right.");

    backgroundo(BACKGROUND_HERMIT, "hermit", ABILITY_HEAL, ABILITY_RELIGION, FEAT_BG_HERMIT,
        "You lived in seclusion—either in a sheltered community such as a monastery, or entirely alone-for "
        "a formative part of your life. In your time apart from the clamor of society, you found quiet, "
        "solitude, and perhaps some of the answers you were looking for.");

    backgroundo(BACKGROUND_SQUIRE, "squire", ABILITY_HISTORY, ABILITY_DIPLOMACY, FEAT_BG_SQUIRE,
        "You trained at the feet of a knight, maintaining their gear and supporting them at "
        "tourneys and in battle. Now you search for a challenge that will prove you worthy of "
        "full knighthood, or you’ve spurned pomp and ceremony to test yourself in honest, albeit "
        "less formal, combat.");

    backgroundo(BACKGROUND_NOBLE, "noble", ABILITY_HISTORY, ABILITY_DIPLOMACY, FEAT_BG_NOBLE,
        "You understand wealth, power, and privilege. You carry a noble title, and your family "
        "owns land, collects taxes, and wields significant political influence. You might be a "
        "pampered aristocrat unfamiliar with work or discomfort, a former merchant just elevated "
        "to the nobility, or a disinherited scoundrel with a disproportionate sense of entitlement. "
        "Or you could be an honest, hard-working landowner who cares deeply about the people who live "
        "and work on your land, keenly aware of your responsibility to them.");

    backgroundo(BACKGROUND_OUTLANDER, "outlander", ABILITY_ATHLETICS, ABILITY_SURVIVAL, FEAT_BG_OUTLANDER,
        "You grew up in the wilds, far from the comforts of town and technology. You’ve witnessed the "
        "migration of herds larger than forests, survived weather more extreme than any city-dweller "
        "could comprehend, and enjoyed the solitude of being the only thinking creature for miles in "
        "any direction. The wilds are in your blood, whether you were a nomad, an explorer, a recluse, "
        "a hunter-gatherer, or even a marauder. Even in places where you don’t know the specific features "
        "of the terrain, you know the ways of the wild.");

    backgroundo(BACKGROUND_PIRATE, "pirate", ABILITY_ATHLETICS, ABILITY_PERCEPTION, FEAT_BG_PIRATE,
        "You sailed on a seagoing vessel for years. In that time, you faced down mighty storms, monsters "
        "of the deep, and those who wanted to sink your craft to the bottomless depths. Your first love is "
        "the distant line of the horizon, but the time has come to try your hand at something new.");

    backgroundo(BACKGROUND_SAGE, "sage", ABILITY_ARCANA, ABILITY_HISTORY, FEAT_BG_SAGE,
        "You spent years learning the lore of the multiverse. You scoured manuscripts, studied scrolls, "
        "and listened to the greatest experts on the subjects that interest you. Your efforts have made "
        "you a master in your fields of study.");
    
    backgroundo(BACKGROUND_SAILOR, "sailor", ABILITY_ATHLETICS, ABILITY_PERCEPTION, FEAT_BG_SAILOR,
        "You sailed on a seagoing vessel for years. In that time, you faced down mighty storms, monsters "
        "of the deep, and those who wanted to sink your craft to the bottomless depths. Your first love is "
        "the distant line of the horizon, but the time has come to try your hand at something new.");

    backgroundo(BACKGROUND_SOLDIER, "soldier", ABILITY_ATHLETICS, ABILITY_INTIMIDATE, FEAT_BG_SOLDIER,
        "War has been your life for as long as you care to remember. You trained as a youth, studied the "
        "use of weapons and armor, learned basic survival techniques, including how to stay alive on the "
        "battlefield. You might have been part of a standing national army or a mercenary company, or "
        "perhaps a member of a local militia who rose to prominence during a recent war.");

    backgroundo(BACKGROUND_URCHIN, "urchin", ABILITY_SLEIGHT_OF_HAND, ABILITY_STEALTH, FEAT_BG_URCHIN,
        "You grew up on the streets alone, orphaned, and poor. You had no one to watch over you or to "
        "provide for you, so you learned to provide for yourself. You fought fiercely over food and kept "
        "a constant watch out for other desperate souls who might steal from you. You slept on rooftops "
        "and in alleyways, exposed to the elements, and endured sickness without the advantage of medicine "
        "or a place to recuperate. You’ve survived despite all odds, and did so through cunning, strength, "
        "speed, or some combination of each.");
}

#define TEMPLE_COST_CURE    100
#define TEMPLE_COST_HEAL    500
#define TEMPLE_COST_REMOVE_POISON   250
#define TEMPLE_COST_REMOVE_DISEASE  350
#define TEMPLE_COST_REMOVE_CURSE    350
#define TEMPLE_COST_REJUV   100
#define TEMPLE_COST_RESTORATION 250
#define TEMPLE_COST_REMOVE_BLINDNESS 50
#define TEMPLE_COST_REMOVE_DEAFNESS 50

bool has_acolyte_in_group(struct char_data *ch)
{
    struct char_data *tch = NULL;
    bool acolyte = false;

    if (!ch || IN_ROOM(ch) == NOWHERE)
        return false;

    for (tch = world[IN_ROOM(ch)].people; tch; tch = tch->next_in_room)
    {
        if (IS_NPC(tch)) continue;
        if (!is_player_grouped(tch, ch)) continue;
        if (HAS_FEAT(tch, FEAT_BG_ACOLYTE))
        {
            acolyte = true;
            break;
        }
    }
    return acolyte;
}

bool temple_blessing_cost_handling(struct char_data *ch, int blessing)
{
    if (GET_GOLD(ch) < blessing && !has_acolyte_in_group(ch))
    {
        send_to_char(ch, "You need %d gold to purchase this blessing.\r\n", blessing);
        return false;
    }

    send_to_char(ch, "You receive the requested blessing.\r\n");
    act("$n is blessed by the temple priest.", FALSE, ch, 0, 0, TO_ROOM);

    if (!has_acolyte_in_group(ch))
    {
        GET_GOLD(ch) -= blessing;
        send_to_char(ch, "The blessing costs you %d gold coins.\r\n", blessing);
    }

    return true;
}

SPECIAL(temple)
{
    if (!CMD_IS("temple"))
        return 0;

    char arg[200];
    struct char_data *temple = (struct char_data *) me;

    one_argument(argument, arg, sizeof(arg));

    if (!*arg)
    {
        send_to_char(ch, "Please select from the following options:\r\n");
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "cure", "casts cure critical wounds", TEMPLE_COST_CURE);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "heal", "casts heal", TEMPLE_COST_HEAL);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "vision", "casts cure blindness", TEMPLE_COST_REMOVE_BLINDNESS);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "hearing", "casts cure deafness", TEMPLE_COST_REMOVE_DEAFNESS);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "poison", "casts remove poison", TEMPLE_COST_REMOVE_POISON);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "disease", "casts cure disease", TEMPLE_COST_REMOVE_DISEASE);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "curse", "casts remove curse", TEMPLE_COST_REMOVE_CURSE);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "restore", "casts restoration", TEMPLE_COST_RESTORATION);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "stamina", "casts vigorize critical", TEMPLE_COST_REJUV);
        send_to_char(ch, "\r\n");
        return 1;
    }
    else if (is_abbrev(arg, "cure"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_CURE))
        {
            call_magic(temple, ch, 0, SPELL_CURE_CRITIC, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else if (is_abbrev(arg, "heal"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_HEAL))
        {
            call_magic(temple, ch, 0, SPELL_HEAL, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else if (is_abbrev(arg, "vision"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_REMOVE_BLINDNESS))
        {
            call_magic(temple, ch, 0, SPELL_CURE_BLIND, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else if (is_abbrev(arg, "hearing"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_REMOVE_DEAFNESS))
        {
            call_magic(temple, ch, 0, SPELL_CURE_DEAFNESS, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else if (is_abbrev(arg, "poison"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_REMOVE_POISON))
        {
            call_magic(temple, ch, 0, SPELL_REMOVE_POISON, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else if (is_abbrev(arg, "disease"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_REMOVE_DISEASE))
        {
            call_magic(temple, ch, 0, SPELL_REMOVE_DISEASE, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else if (is_abbrev(arg, "curse"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_REMOVE_CURSE))
        {
            call_magic(temple, ch, 0, SPELL_REMOVE_CURSE, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else if (is_abbrev(arg, "restore"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_RESTORATION))
        {
            call_magic(temple, ch, 0, SPELL_RESTORATION, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else if (is_abbrev(arg, "stamina"))
    {
        if (temple_blessing_cost_handling(ch, TEMPLE_COST_REJUV))
        {
            call_magic(temple, ch, 0, SPELL_VIGORIZE_CRITICAL, 0, GET_LEVEL(temple), CAST_SPELL);
            return 1;
        }
    }
    else
    {
        send_to_char(ch, "Please select from the following options:\r\n");
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "cure", "casts cure critical wounds", TEMPLE_COST_CURE);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "heal", "casts heal", TEMPLE_COST_HEAL);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "vision", "casts cure blindness", TEMPLE_COST_REMOVE_BLINDNESS);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "hearing", "casts cure deafness", TEMPLE_COST_REMOVE_DEAFNESS);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "poison", "casts remove poison", TEMPLE_COST_REMOVE_POISON);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "disease", "casts cure disease", TEMPLE_COST_REMOVE_DISEASE);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "curse", "casts remove curse", TEMPLE_COST_REMOVE_CURSE);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "restore", "casts restoration", TEMPLE_COST_RESTORATION);
        send_to_char(ch, "-- %-15s %-30s%-d coins\r\n", "stamina", "casts vigorize critical", TEMPLE_COST_REJUV);
        send_to_char(ch, "\r\n");
        return 1;
    }
    return 1;
}

ACMDU(do_swindle)
{
  struct char_data *vict = NULL;
  int grade, gold;
  char buf[200];

  if (!HAS_REAL_FEAT(ch, FEAT_BG_CHARLATAN))
  {
    send_to_char(ch, "Only charlatans can 'swindle'.\r\n");
    return;
  }

  skip_spaces(&argument);

  if (!*argument)
  {
    send_to_char(ch, "Who do you want to swindle?\r\n");
    return;
  }

  if (!(vict = get_char_vis(ch, argument, NULL, FIND_CHAR_ROOM)))
  {
    send_to_char(ch, "You don't see anyone here by that description.\r\n");
    return;
  }

  if (!IS_NPC(vict))
  {
    send_to_char(ch, "You can't swindle players.\r\n");
    return;
  }

  if (GET_RACE(vict) != RACE_TYPE_HUMANOID && GET_RACE(vict) != RACE_TYPE_GIANT && 
        GET_RACE(vict) != RACE_TYPE_MONSTROUS_HUMANOID && GET_RACE(vict) != RACE_TYPE_DRAGON &&
        GET_RACE(vict) != RACE_TYPE_FEY && GET_RACE(vict) != RACE_TYPE_LYCANTHROPE &&
        GET_RACE(vict) != RACE_TYPE_OUTSIDER)
  {
    send_to_char(ch, "They aren't intelligent enough to swindle.\r\n");
    return;
  }

  if (vict->char_specials.has_swindle_been_attempted)
  {
    act("$N is already wary of being swindled.", TRUE, ch, 0, vict, TO_CHAR);
    return;
  }

    // you can only try once;
    vict->char_specials.has_swindle_been_attempted = true;

  int skill = skill_roll(ch, ABILITY_DECEPTION);
  int dc = skill_roll(vict, ABILITY_INSIGHT);

  if (skill < dc)
  {
    appear(ch, FALSE);
    act("$N recognizes you con and attacks!", FALSE, ch, 0, vict, TO_CHAR);
    act("You recognize $n's con!", FALSE, ch, 0, vict, TO_VICT);
    act("$N recognizes $n's con and attacks!", FALSE, ch, 0, vict, TO_NOTVICT);
    hit(vict, ch, TYPE_UNDEFINED, DAM_RESERVED_DBC, 0, FALSE);

    return;
  }
  else
  {    
    gold = dice(1, GET_LEVEL(vict)) * 5;
    snprintf(buf, sizeof(buf), "You manage to swindle $N out of %d coins.", gold);
    act(buf, FALSE, ch, 0, vict, TO_CHAR);
    snprintf(buf, sizeof(buf), "$n convinces you to give $m %d coins.", gold);
    act(buf, FALSE, ch, 0, vict, TO_VICT);
    act("$n convinces $N to give $M some coins.", FALSE, ch, 0, vict, TO_NOTVICT);

    if (dice(1, 20) == 1)
    {
        ch->char_specials.which_treasure_message = CUSTOM_TREASURE_MESSAGE_SWINDLE;
        grade = quick_grade_check(GET_LEVEL(vict));
        switch(dice(1, 10))
        {
            case 1:
            award_random_crystal(ch, grade);
            break;
            case 2: case 3:
            award_expendable_item(ch, grade, TYPE_SCROLL);
            break;
            case 5:  case 6:  case 7: 
            award_expendable_item(ch, grade, TYPE_POTION);
            break;
            case 8: 
            award_expendable_item(ch, grade, TYPE_WAND);
            break;
            case 9: 
            award_expendable_item(ch, grade, TYPE_STAFF);
            break;
            case 10: 
            award_misc_magic_item(ch, determine_rnd_misc_cat(), cp_convert_grade_enchantment(grade));
            break;
        }
        ch->char_specials.which_treasure_message = CUSTOM_TREASURE_MESSAGE_NONE;
        return;
    }
  }
}