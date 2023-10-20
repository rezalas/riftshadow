--
-- Dumping data for table `interp_table`
--
LOCK TABLES `interp_table` WRITE;

/*!40000 ALTER TABLE `interp_table` DISABLE KEYS */;

INSERT INTO
	`interp_table`
VALUES
	(1, 'north', 0, 'do_north', 0, 2, 2),
	(2, 'east', 0, 'do_east', 0, 2, 2),
	(3, 'south', 0, 'do_south', 0, 2, 2),
	(4, 'west', 0, 'do_west', 0, 2, 2),
	(5, 'up', 0, 'do_up', 0, 2, 2),
	(6, 'down', 0, 'do_down', 0, 2, 2),
	(7, 'affects', 0, 'do_affects', 0, 0, 0),
	(8, 'at', 0, 'do_at', 54, 0, 0),
	(9, 'cast', 0, 'do_cast', 0, 0, 1),
	(10, 'call', 0, 'do_call', 0, 0, 1),
	(11, 'commune', 0, 'do_commune', 0, 0, 1),
	(12, 'rest', 0, 'do_rest', 0, 0, 0),
	(13, 'rune', 0, 'do_rune', 0, 0, 1),
	(14, 'raffects', 0, 'do_raffects', 54, 0, 0),
	(15, 'buy', 0, 'do_buy', 0, 0, 1),
	(16, 'channels', 0, 'do_channels', 0, 0, 0),
	(17, 'exits', 0, 'do_exits', 0, 0, 0),
	(18, 'get', 0, 'do_get', 0, 0, 0),
	(19, 'goto', 0, 'do_goto', 52, 0, 0),
	(20, 'group', 0, 'do_group', 0, 0, 0),
	(21, 'balance', 0, 'do_balance', 0, 0, 1),
	(22, 'deposit', 0, 'do_deposit', 0, 0, 1),
	(23, 'ratings', 0, 'do_ratings', 55, 0, 0),
	(24, 'hit', 0, 'do_hit', 0, 0, 3),
	(25, 'look', 0, 'do_look', 0, 0, 0),
	(26, 'lore', 0, 'do_lore', 0, 0, 0),
	(27, 'cleader', 0, 'do_leader', 58, 1, 0),
	(28, 'inventory', 0, 'do_inventory', 0, 0, 0),
	(29, 'induct', 0, 'do_induct', 0, 1, 0),
	(30, 'kill', 0, 'do_kill', 0, 0, 1),
	(31, 'cb', 0, 'do_cb', 0, 0, 0),
	(32, '/', 0, 'do_recall', 0, 0, 1),
	(33, 'order', 0, 'do_order', 0, 0, 1),
	(34, 'practice', 0, 'do_practice', 0, 0, 0),
	(35, 'pray', 0, 'do_pray', 0, 1, 0),
	(36, 'sit', 0, 'do_sit', 0, 0, 0),
	(37, 'sockets', 0, 'do_sockets', 54, 0, 0),
	(38, 'extitle', 0, 'do_extitle', 55, 0, 0),
	(39, 'multicheck', 0, 'do_multicheck', 54, 0, 0),
	(40, 'stand', 0, 'do_stand', 0, 0, 0),
	(41, 'tell', 0, 'do_tell', 0, 0, 1),
	(42, 'unlock', 0, 'do_unlock', 0, 0, 1),
	(43, 'wield', 0, 'do_wear', 0, 0, 1),
	(44, 'withdraw', 0, 'do_withdraw', 0, 0, 1),
	(45, 'wizhelp', 0, 'do_wizhelp', 52, 0, 0),
	(46, 'request', 0, 'do_request', 0, 1, 1),
	(47, 'demand', 0, 'do_demand', 0, 0, 1),
	(48, 'areas', 0, 'do_areas', 0, 0, 0),
	(49, 'bug', 0, 'do_bug', 0, 0, 0),
	(50, 'changes', 0, 'do_changes', 0, 0, 0),
	(51, 'commands', 0, 'do_commands', 0, 0, 0),
	(52, 'compare', 0, 'do_compare', 0, 0, 0),
	(53, 'consider', 0, 'do_consider', 0, 0, 0),
	(54, 'count', 0, 'do_count', 0, 0, 0),
	(55, 'creds', 0, 'do_credits', 0, 0, 0),
	(56, 'credits', 0, 'do_diku', 0, 0, 0),
	(57, 'equipment', 0, 'do_equipment', 0, 0, 0),
	(58, 'examine', 0, 'do_examine', 0, 0, 0),
	(59, 'glance', 0, 'do_glance', 0, 0, 0),
	(60, 'oldhelp', 0, 'do_oldhelp', 0, 0, 0),
	(61, 'help', 0, 'do_help', 0, 0, 0),
	(62, 'idea', 0, 'do_idea', 0, 0, 0),
	(63, 'info', 0, 'do_groups', 0, 0, 0),
	(64, 'motd', 0, 'do_motd', 0, 0, 0),
	(65, 'news', 0, 'do_news', 0, 0, 0),
	(66, 'powers', 0, 'do_powers', 0, 0, 0),
	(67, 'supplications', 0, 'do_supps', 0, 0, 0),
	(68, 'read', 0, 'do_look', 0, 0, 0),
	(69, 'report', 0, 'do_report', 0, 0, 1),
	(70, 'rules', 0, 'do_rules', 0, 0, 0),
	(71, 'score', 0, 'do_score', 0, 0, 0),
	(72, 'skills', 0, 'do_skills', 0, 0, 0),
	(73, 'socials', 0, 'do_socials', 0, 0, 0),
	(74, 'show', 0, 'do_show', 0, 0, 0),
	(75, 'spells', 0, 'do_spells', 0, 0, 0),
	(76, 'story', 0, 'do_story', 0, 0, 0),
	(77, 'time', 0, 'do_time', 0, 0, 0),
	(78, 'typo', 0, 'do_typo', 0, 0, 0),
	(79, 'vote', 0, 'do_vote', 0, 1, 0),
	(80, 'weather', 0, 'do_weather', 0, 0, 0),
	(81, 'who', 0, 'do_who', 0, 0, 0),
	(82, 'whois', 0, 'do_whois', 0, 0, 0),
	(83, 'wizlist', 0, 'do_wizlist', 0, 0, 0),
	(84, 'worth', 0, 'do_worth', 0, 0, 0),
	(85, 'alias', 0, 'do_alias', 0, 0, 0),
	(86, 'autolist', 0, 'do_autolist', 0, 0, 0),
	(87, 'autoabort', 0, 'do_autoabort', 0, 0, 0),
	(88, 'autoassist', 0, 'do_autoassist', 0, 0, 0),
	(89, 'autoexit', 0, 'do_autoexit', 0, 0, 0),
	(90, 'autogold', 0, 'do_autogold', 0, 0, 0),
	(91, 'autoloot', 0, 'do_autoloot', 0, 0, 0),
	(92, 'autosac', 0, 'do_autosac', 0, 0, 0),
	(93, 'autosplit', 0, 'do_autosplit', 0, 0, 0),
	(94, 'brief', 0, 'do_brief', 0, 0, 0),
	(95, 'chess', 0, 'do_chess', 0, 0, 0),
	(96, 'color', 0, 'do_color', 0, 0, 0),
	(97, 'combine', 0, 'do_combine', 0, 0, 0),
	(98, 'compact', 0, 'do_compact', 0, 0, 0),
	(99, 'description', 0, 'do_description', 0, 0, 0),
	(100, 'delete', 0, 'do_delete', 0, 1, 0),
	(101, 'nofollow', 0, 'do_nofollow', 0, 0, 0),
	(102, 'nosummon', 0, 'do_nosummon', 0, 0, 0),
	(103, 'outfit', 0, 'do_outfit', 0, 0, 0),
	(104, 'password', 0, 'do_password', 0, 2, 0),
	(105, 'prompt', 0, 'do_prompt', 0, 0, 0),
	(106, 'role', 0, 'do_role', 0, 0, 0),
	(107, 'scroll', 0, 'do_scroll', 0, 0, 0),
	(108, 'specialize', 0, 'do_specialize', 0, 0, 0),
	(109, 'title', 0, 'do_title', 55, 0, 0),
	(110, 'unalias', 0, 'do_unalias', 0, 0, 0),
	(111, 'wimpy', 0, 'do_wimpy', 0, 0, 0),
	(112, 'deaf', 0, 'do_deaf', 0, 0, 0),
	(113, 'emote', 0, 'do_emote', 0, 0, 0),
	(114, 'pmote', 0, 'do_pmote', 0, 0, 0),
	(115, ',', 0, 'do_emote', 0, 0, 2),
	(116, 'gtell', 0, 'do_gtell', 0, 0, 1),
	(117, ';', 0, 'do_gtell', 0, 0, 3),
	(118, 'builder', 0, 'do_builder', 0, 0, 3),
	(119, '.', 0, 'do_builder', 0, 0, 3),
	(120, 'immtalk', 0, 'do_immtalk', 53, 0, 0),
	(121, 'note', 0, 'do_note', 0, 0, 0),
	(122, 'noreply', 0, 'do_noreply', 0, 0, 0),
	(123, 'newbie', 0, 'do_newbie', 0, 0, 0),
	(124, 'reply', 0, 'do_reply', 0, 0, 1),
	(125, 'replay', 0, 'do_replay', 0, 0, 0),
	(126, 'say', 0, 'do_say', 0, 0, 1),
	(127, '\'', 0, 'do_say', 0, 0, 3),
	(128, 'sing', 0, 'do_sing', 0, 0, 1),
	(129, 'whisper', 0, 'do_whisper', 0, 0, 0),
	(130, '[', 0, 'do_whisper', 0, 0, 0),
	(131, 'unread', 0, 'do_unread', 0, 0, 0),
	(132, 'yell', 0, 'do_yell', 0, 0, 1),
	(133, 'brandish', 0, 'do_brandish', 0, 0, 1),
	(134, 'close', 0, 'do_close', 0, 0, 1),
	(135, 'drink', 0, 'do_drink', 0, 0, 1),
	(136, 'drop', 0, 'do_drop', 0, 0, 0),
	(137, 'embalm', 0, 'do_embalm', 0, 0, 3),
	(138, 'eat', 0, 'do_eat', 0, 0, 1),
	(139, 'envenom', 0, 'do_envenom', 0, 0, 1),
	(140, 'fill', 0, 'do_fill', 0, 0, 1),
	(141, 'give', 0, 'do_give', 0, 0, 1),
	(142, 'heal', 0, 'do_heal', 0, 0, 1),
	(143, 'hold', 0, 'do_wear', 0, 0, 1),
	(144, 'list', 0, 'do_list', 0, 0, 0),
	(145, 'lock', 0, 'do_lock', 0, 0, 1),
	(146, 'open', 0, 'do_open', 0, 0, 1),
	(147, 'pick', 0, 'do_pick', 0, 0, 1),
	(148, 'pour', 0, 'do_pour', 0, 0, 1),
	(149, 'put', 0, 'do_put', 0, 0, 0),
	(150, 'quaff', 0, 'do_quaff', 0, 0, 1),
	(151, 'quiet', 0, 'do_quiet', 0, 0, 0),
	(152, 'recite', 0, 'do_recite', 0, 0, 1),
	(153, 'remove', 0, 'do_remove', 0, 0, 1),
	(154, 'roll', 0, 'do_roll', 0, 0, 1),
	(155, 'doorbash', 0, 'do_door_bash', 0, 0, 1),
	(156, 'headbutt', 0, 'do_headbutt', 0, 0, 1),
	(
		157,
		'silentmovement',
		0,
		'do_silent_movement',
		0,
		0,
		0
	),
	(158, 'gore', 0, 'do_gore', 0, 0, 1),
	(159, 'sell', 0, 'do_sell', 0, 0, 1),
	(160, 'take', 0, 'do_get', 0, 0, 1),
	(161, 'sacrifice', 0, 'do_sacrifice', 0, 0, 1),
	(162, 'junk', 0, 'do_sacrifice', 0, 0, 3),
	(163, 'butcher', 0, 'do_butcher', 0, 0, 3),
	(164, 'skin', 0, 'do_skin', 0, 0, 3),
	(165, 'value', 0, 'do_value', 0, 0, 0),
	(166, 'wear', 0, 'do_wear', 0, 0, 1),
	(167, 'listvotes', 0, 'do_listvotes', 0, 0, 0),
	(168, 'zap', 0, 'do_zap', 0, 0, 1),
	(169, 'vanish', 0, 'do_vanish', 0, 0, 1),
	(170, 'aura', 0, 'do_aura_of_sustenance', 0, 0, 1),
	(171, 'wake', 0, 'do_wake', 0, 0, 0),
	(172, 'style', 0, 'do_style', 0, 0, 2),
	(173, 'cleave', 0, 'do_cleave', 0, 0, 2),
	(174, 'herb', 0, 'do_herb', 0, 0, 2),
	(175, 'records', 0, 'do_records', 0, 0, 2),
	(176, 'ambush', 0, 'do_ambush', 0, 0, 3),
	(177, 'pugil', 0, 'do_pugil', 0, 0, 3),
	(178, 'lash', 0, 'do_lash', 0, 0, 3),
	(
		179,
		'shieldcleave',
		0,
		'do_shield_cleave',
		0,
		0,
		3
	),
	(180, 'backstab', 0, 'do_backstab', 0, 0, 1),
	(181, 'knife', 0, 'do_knife', 0, 0, 1),
	(182, 'bash', 0, 'do_bash', 0, 0, 1),
	(183, 'throw', 0, 'do_throw', 0, 0, 1),
	(184, 'strangle', 0, 'do_strangle', 0, 0, 0),
	(185, 'blindness', 0, 'do_blindness_dust', 0, 0, 3),
	(186, 'poison', 0, 'do_poison_dust', 0, 0, 3),
	(187, 'warcry', 0, 'do_warcry', 0, 0, 1),
	(188, 'endure', 0, 'do_endure', 0, 0, 1),
	(189, 'nerve', 0, 'do_nerve', 0, 0, 1),
	(190, 'tail', 0, 'do_tail', 0, 0, 3),
	(191, 'blackjack', 0, 'do_blackjack', 0, 0, 0),
	(192, 'berserk', 0, 'do_berserk', 0, 0, 1),
	(193, 'circle', 0, 'do_circle_stab', 0, 0, 1),
	(194, 'darksight', 0, 'do_darksight', 0, 0, 1),
	(195, 'detect', 0, 'do_detect_hidden', 0, 0, 0),
	(196, 'dirt', 0, 'do_dirt', 0, 0, 1),
	(197, 'disarm', 0, 'do_disarm', 0, 0, 1),
	(198, 'disengage', 0, 'do_disengage', 0, 0, 1),
	(199, 'flee', 0, 'do_flee', 0, 0, 1),
	(200, 'kick', 0, 'do_kick', 0, 0, 1),
	(201, 'murder', 0, 'do_murder', 0, 0, 1),
	(202, 'rescue', 0, 'do_rescue', 0, 0, 3),
	(203, 'trip', 0, 'do_trip', 0, 0, 1),
	(204, 'defend', 0, 'do_defend', 0, 0, 0),
	(205, 'assassinate', 0, 'do_assassinate', 0, 0, 3),
	(206, 'forage', 0, 'do_forage', 0, 0, 1),
	(
		207,
		'protection',
		0,
		'do_protection_heat_cold',
		0,
		0,
		1
	),
	(208, 'intimidate', 0, 'do_intimidate', 0, 0, 1),
	(209, 'bind', 0, 'do_bind', 0, 0, 2),
	(210, 'unbind', 0, 'do_unbind', 0, 0, 3),
	(211, 'charge', 0, 'do_charge', 0, 0, 2),
	(212, 'offhand', 0, 'do_offhand', 0, 0, 2),
	(213, 'enter', 0, 'do_enter', 0, 2, 2),
	(214, 'storytell', 0, 'do_storytell', 0, 2, 2),
	(215, 'entrap', 0, 'do_entrap', 0, 0, 2),
	(216, 'drive', 0, 'do_drive', 0, 0, 1),
	(217, 'dash', 0, 'do_dash', 0, 0, 1),
	(218, 'howl', 0, 'do_howl', 0, 0, 1),
	(219, 'hobble', 0, 'do_hobble', 0, 0, 2),
	(
		220,
		'cripplingblow',
		0,
		'do_crippling_blow',
		0,
		0,
		2
	),
	(221, 'gouge', 0, 'do_gouge', 0, 0, 1),
	(222, 'bleed', 0, 'do_bleed', 0, 0, 1),
	(223, 'posture', 0, 'do_posture', 0, 0, 2),
	(224, 'unbalance', 0, 'do_unbalance', 0, 0, 2),
	(225, 'drain', 0, 'do_drain', 0, 0, 2),
	(226, 'concuss', 0, 'do_concuss', 0, 0, 2),
	(227, 'retreat', 0, 'do_retreat', 0, 0, 2),
	(228, 'overhead', 0, 'do_overhead', 0, 0, 2),
	(229, 'uppercut', 0, 'do_uppercut', 0, 0, 2),
	(230, 'disrupt', 0, 'do_disrupt', 0, 0, 2),
	(231, 'dart', 0, 'do_dart', 0, 0, 2),
	(232, 'hurl', 0, 'do_hurl', 0, 0, 2),
	(233, 'extract', 0, 'do_extract', 0, 0, 1),
	(234, 'exchange', 0, 'do_exchange', 0, 0, 2),
	(235, 'shieldbash', 0, 'do_shieldbash', 0, 0, 3),
	(236, 'brace', 0, 'do_brace', 0, 0, 3),
	(237, 'shatter', 0, 'do_shatter', 0, 0, 2),
	(238, 'whirlwind', 0, 'do_whirlwind', 0, 0, 2),
	(239, 'leadership', 0, 'do_leadership', 0, 0, 2),
	(240, 'uncoil', 0, 'do_uncoil', 0, 0, 3),
	(241, 'entwine', 0, 'do_entwine', 0, 0, 3),
	(242, 'pull', 0, 'do_pull', 0, 0, 3),
	(243, 'assess', 0, 'do_assess', 0, 0, 3),
	(244, 'exploit', 0, 'do_exploit', 0, 0, 3),
	(245, 'outflank', 0, 'do_outflank', 0, 0, 2),
	(246, 'impale', 0, 'do_impale', 0, 0, 3),
	(
		247,
		'valiantcharge',
		0,
		'do_valiant_charge',
		0,
		0,
		3
	),
	(248, 'turnundead', 0, 'do_turn_undead', 0, 0, 2),
	(
		249,
		'strikeofvirtue',
		0,
		'do_strike_of_virtue',
		0,
		0,
		2
	),
	(
		250,
		'groupretreat',
		0,
		'do_group_retreat',
		0,
		0,
		2
	),
	(251, 'flip', 0, 'do_flip', 0, 0, 1),
	(252, 'follow', 0, 'do_follow', 0, 0, 0),
	(253, 'creep', 0, 'do_creep', 0, 0, 0),
	(254, 'scan', 0, 'do_scan', 0, 0, 0),
	(255, 'gain', 0, 'do_gain', 0, 0, 0),
	(256, 'groups', 0, 'do_groups', 0, 0, 0),
	(257, 'acute', 0, 'do_acute_vision', 0, 0, 0),
	(258, 'barkskin', 0, 'do_barkskin', 0, 0, 0),
	(259, 'animal', 0, 'do_animal_call', 0, 0, 0),
	(260, 'enlist', 0, 'do_enlist', 0, 0, 0),
	(261, 'camouflage', 0, 'do_camouflage', 0, 0, 0),
	(262, 'camp', 0, 'do_camp', 0, 0, 1),
	(263, 'hide', 0, 'do_hide', 0, 0, 0),
	(264, 'laying', 0, 'do_laying_hands', 0, 0, 0),
	(265, 'quit', 0, 'do_quit', 0, 0, 0),
	(266, 'release', 0, 'do_release', 0, 0, 1),
	(267, 'recall', 0, 'do_recall', 0, 0, 1),
	(268, 'tame', 0, 'do_tame', 0, 0, 1),
	(269, 'find water', 0, 'do_find_water', 0, 0, 1),
	(270, 'fw', 0, 'do_fw', 0, 0, 3),
	(271, 'rent', 0, 'do_rent', 0, 0, 2),
	(272, 'save', 0, 'do_save', 0, 0, 0),
	(273, 'sleep', 0, 'do_sleep', 0, 0, 0),
	(274, 'sneak', 0, 'do_sneak', 0, 0, 0),
	(275, 'split', 0, 'do_split', 0, 0, 1),
	(276, 'steal', 0, 'do_steal', 0, 2, 0),
	(277, 'plant', 0, 'do_plant', 0, 2, 0),
	(278, 'gag', 0, 'do_gag', 0, 2, 0),
	(279, 'ungag', 0, 'do_ungag', 0, 2, 0),
	(280, 'drag', 0, 'do_drag', 0, 2, 0),
	(281, 'sign', 0, 'do_sign', 0, 2, 0),
	(282, 'slash', 0, 'do_slash', 0, 2, 0),
	(283, 'tripwire', 0, 'do_tripwire', 0, 2, 0),
	(284, 'stash', 0, 'do_stash', 0, 2, 0),
	(285, 'disguise', 0, 'do_disguise', 0, 2, 1),
	(286, 'undisguise', 0, 'do_undisguise', 0, 2, 1),
	(287, 'shadowcloak', 0, 'do_shadow_cloak', 0, 2, 1),
	(288, 'search', 0, 'do_search', 0, 2, 0),
	(289, 'counterfeit', 0, 'do_counterfeit', 0, 2, 0),
	(290, 'strip', 0, 'do_strip', 0, 2, 0),
	(291, 'train', 0, 'do_train', 0, 0, 0),
	(292, 'visible', 0, 'do_visible', 0, 0, 0),
	(293, 'where', 0, 'do_where', 0, 0, 0),
	(294, 'cloak', 0, 'do_cloak', 0, 0, 0),
	(295, 'hometown', 0, 'do_hometown', 0, 1, 1),
	(296, 'invoke', 0, 'do_invoke', 0, 0, 1),
	(297, 'snare', 0, 'do_snare', 0, 0, 2),
	(298, 'bearcall', 0, 'do_bear_call', 0, 1, 2),
	(299, 'bounty', 0, 'do_bounty', 0, 1, 2),
	(300, 'topbounties', 0, 'do_topbounties', 55, 0, 2),
	(301, 'offer', 0, 'do_offer', 0, 1, 2),
	(302, 'listoffer', 0, 'do_listoffer', 53, 0, 2),
	(303, 'taunt', 0, 'do_taunt', 0, 0, 1),
	(304, 'consume', 0, 'do_consume', 0, 0, 1),
	(
		305,
		'breathofmephisto',
		0,
		'do_breath_mephisto',
		0,
		0,
		1
	),
	(306, 'touch', 0, 'do_touch', 0, 0, 1),
	(307, 'exile', 0, 'do_exile', 0, 1, 1),
	(308, 'trustgroup', 0, 'do_trustgroup', 0, 0, 0),
	(309, 'trustcabal', 0, 'do_trustcabal', 0, 0, 0),
	(310, 'trustall', 0, 'do_trustall', 0, 0, 0),
	(311, 'advance', 0, 'do_advance', 60, 1, 0),
	(312, 'alist', 0, 'do_alist', 52, 0, 0),
	(313, 'dump', 0, 'do_dump', 60, 1, 2),
	(314, 'immtrust', 0, 'do_immtrust', 60, 1, 0),
	(315, 'violate', 0, 'do_violate', 60, 1, 0),
	(316, 'allow', 0, 'do_allow', 58, 1, 0),
	(317, 'ban', 0, 'do_ban', 58, 1, 0),
	(318, 'rinfo', 0, 'do_rinfo', 54, 2, 1),
	(319, 'deny', 0, 'do_deny', 58, 1, 0),
	(320, 'disconn', 0, 'do_disconnect', 55, 1, 0),
	(321, 'flag', 0, 'do_flag', 57, 1, 0),
	(322, 'freeze', 0, 'do_freeze', 57, 1, 0),
	(323, 'lag', 0, 'do_lag', 60, 1, 0),
	(324, 'global', 0, 'do_global', 54, 0, 2),
	(325, 'smite', 0, 'do_smite', 55, 1, 0),
	(326, 'permban', 0, 'do_permban', 59, 1, 0),
	(327, 'iprotect', 0, 'do_protect', 59, 1, 0),
	(328, 'reboot', 0, 'do_reboot', 55, 1, 0),
	(329, 'set', 0, 'do_set', 58, 1, 0),
	(330, 'wizlock', 0, 'do_wizlock', 58, 1, 0),
	(331, 'classes', 0, 'do_classes', 52, 0, 0),
	(332, 'force', 0, 'do_force', 53, 1, 0),
	(333, 'load', 0, 'do_load', 55, 1, 0),
	(334, 'newlock', 0, 'do_newlock', 58, 1, 0),
	(335, 'nochann', 0, 'do_nochannels', 55, 1, 0),
	(336, 'noemote', 0, 'do_noemote', 55, 1, 0),
	(337, 'noshout', 0, 'do_noshout', 55, 1, 0),
	(338, 'notell', 0, 'do_notell', 55, 1, 0),
	(339, 'pecho', 0, 'do_pecho', 53, 1, 0),
	(340, 'pardon', 0, 'do_pardon', 57, 1, 0),
	(341, 'purge', 0, 'do_purge', 56, 1, 0),
	(342, 'restore', 0, 'do_restore', 57, 1, 0),
	(343, 'slay', 0, 'do_slay', 57, 1, 0),
	(344, 'teleport', 0, 'do_transfer', 55, 1, 0),
	(345, 'transfer', 0, 'do_transfer', 55, 1, 0),
	(346, 'poofin', 0, 'do_bamfin', 52, 0, 0),
	(347, 'addapply', 0, 'do_addapply', 58, 0, 0),
	(348, 'maxlimits', 0, 'do_max_limits', 57, 1, 0),
	(349, 'poofout', 0, 'do_bamfout', 52, 0, 0),
	(350, 'freset', 0, 'do_force_reset', 51, 1, 0),
	(351, 'gecho', 0, 'do_gecho', 55, 1, 0),
	(352, 'astrip', 0, 'do_astrip', 55, 1, 0),
	(353, 'tattoo', 0, 'do_brands', 53, 1, 0),
	(354, 'untattoo', 0, 'do_unbrands', 53, 1, 0),
	(355, 'holylight', 0, 'do_holylight', 52, 0, 0),
	(356, 'incognito', 0, 'do_incognito', 54, 0, 0),
	(357, 'invis', 0, 'do_invis', 53, 0, 2),
	(358, 'log', 0, 'do_log', 59, 1, 0),
	(359, 'memory', 0, 'do_memory', 55, 0, 0),
	(360, 'mwhere', 0, 'do_mwhere', 55, 0, 0),
	(361, 'owhere', 0, 'do_owhere', 55, 1, 0),
	(362, 'peace', 0, 'do_peace', 54, 0, 0),
	(363, 'praclist', 0, 'do_praclist', 52, 0, 0),
	(364, 'ghost', 0, 'do_ghost', 54, 1, 0),
	(365, 'echo', 0, 'do_recho', 54, 1, 0),
	(366, 'return', 0, 'do_return', 53, 0, 0),
	(367, 'snoop', 0, 'do_snoop', 55, 1, 0),
	(368, 'stat', 0, 'do_stat', 53, 0, 0),
	(369, 'string', 0, 'do_string', 55, 1, 0),
	(370, 'switch', 0, 'do_switch', 53, 1, 0),
	(371, 'wizinvis', 0, 'do_invis', 54, 0, 0),
	(372, 'vnum', 0, 'do_vnum', 53, 0, 0),
	(373, 'vstat', 0, 'do_vstat', 53, 0, 0),
	(374, 'zecho', 0, 'do_zecho', 54, 1, 0),
	(375, 'clone', 0, 'do_clone', 55, 1, 0),
	(376, 'wiznet', 0, 'do_wiznet', 52, 0, 0),
	(377, 'imotd', 0, 'do_imotd', 52, 0, 0),
	(378, ':', 0, 'do_immtalk', 0, 0, 2),
	(379, 'rename', 0, 'do_rename', 57, 1, 2),
	(380, 'smote', 0, 'do_smote', 52, 0, 0),
	(381, 'prefix', 0, 'do_prefix', 52, 0, 0),
	(382, 'bandage', 0, 'do_bandage', 0, 0, 1),
	(383, 'xlook', 0, 'do_xlook', 53, 0, 0),
	(384, 'affrem', 0, 'do_affrem', 58, 1, 0),
	(
		385,
		'divine',
		0,
		'do_divine_intervention',
		58,
		1,
		0
	),
	(386, 'ccb', 0, 'do_ccb', 53, 1, 0),
	(387, 'heroimm', 0, 'do_heroimm', 58, 1, 0),
	(388, 'history', 0, 'do_history', 53, 1, 0),
	(389, 'finger', 0, 'do_finger', 57, 1, 0),
	(390, 'otrack', 0, 'do_otrack', 54, 1, 0),
	(391, 'ltrack', 0, 'do_ltrack', 60, 1, 0),
	(392, 'sitetrack', 0, 'do_sitetrack', 60, 1, 0),
	(393, 'afk', 0, 'do_afk', 52, 0, 0),
	(394, 'affto', 0, 'do_affto', 58, 0, 0),
	(395, 'crumble', 0, 'do_crumble', 53, 0, 0),
	(396, 'empower', 0, 'do_empower', 53, 1, 0),
	(397, 'pktrack', 0, 'do_pktrack', 53, 1, 0),
	(398, 'ctrack', 0, 'do_ctrack', 55, 1, 0),
	(399, 'pload', 0, 'do_pload', 60, 1, 0),
	(400, 'listhelp', 0, 'do_listhelp', 52, 0, 0),
	(401, 'addhelp', 0, 'do_addhelp', 52, 0, 0),
	(402, 'delhelp', 0, 'do_delhelp', 52, 0, 0),
	(403, 'modhelp', 0, 'do_modhelp', 52, 0, 0),
	(404, 'damage', 0, 'do_damage', 57, 1, 0),
	(405, 'forcetick', 0, 'do_forcetick', 60, 1, 0),
	(406, 'demograph', 0, 'do_demo', 53, 1, 0),
	(407, 'deathmsg', 0, 'do_deathmessage', 52, 1, 0),
	(408, 'rastrip', 0, 'do_rastrip', 55, 1, 0),
	(409, 'oastrip', 0, 'do_oastrip', 55, 1, 0),
	(410, 'aastrip', 0, 'do_aastrip', 55, 1, 0),
	(411, 'damdice', 0, 'do_damdice', 52, 2, 0),
	(412, 'rngtest', 0, 'do_rngtest', 52, 2, 0),
	(413, 'clean', 0, 'do_clean', 60, 1, 0),
	(414, 'gold', 0, 'do_gold', 54, 1, 0),
	(415, 'favor', 0, 'do_devilfavor', 56, 1, 2),
	(416, 'clearfavors', 0, 'do_clearfavors', 58, 1, 2),
	(417, 'gsnlist', 0, 'do_gsnlist', 58, 1, 2),
	(418, 'clearquests', 0, 'do_clearquests', 58, 1, 2),
	(419, 'ccl', 0, 'do_ccl', 58, 2, 2),
	(420, 'antiidle', 0, 'do_antiidle', 52, 2, 2),
	(421, 'noskills', 0, 'do_noskills', 53, 2, 2),
	(422, 'givexp', 0, 'do_givexp', 55, 1, 2),
	(423, 'buglist', 0, 'do_buglist', 52, 0, 0),
	(424, 'questmod', 0, 'do_memtest', 60, 2, 0),
	(425, 'constdump', 0, 'do_constdump', 59, 1, 0),
	(426, 'setpath', 0, 'do_setpath', 53, 1, 0),
	(427, 'racedump', 0, 'do_racedump', 58, 0, 0),
	(428, 'interpdump', 0, 'do_interpdump', 58, 0, 0);

/*!40000 ALTER TABLE `interp_table` ENABLE KEYS */;

UNLOCK TABLES;