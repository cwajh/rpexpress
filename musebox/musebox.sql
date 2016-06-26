create table muns (
	mun_id uuid primary key,
	username varchar(20),
	email varchar(128),
	pwsalt bigint,
	pwhash char(43), -- sha256 base64
	block_acl uuid, -- don't let them see me
	filter_acl uuid, -- don't let me see them
	direct_acl uuid, -- assuming neither blocked nor filtered, who's allowed to direct message?
	default_userpic uuid,
	default_dashboard uuid
	-- contact info?
);
create table muses (
	muse_id uuid primary key
	username varchar(20),
	name varchar(100),
	mun uuid references muns(mun),
	default_avatar uuid,
	p2p_acl uuid,
	front_page uuid
);
create table starters (
	starter_id uuid primary key,
	top_post uuid,
	comm uuid,
	read_acl uuid,	
	contribute_acl uuid,
	title varchar(250),
	single_thread uuid -- if set, that's the only thread.
);

create type tag_pass_mode as enum ('round_robin','free_for_all');
create table threads (
	thread_id uuid primary key,
	starter uuid,
	read_acl uuid,
	write_acl uuid, -- should this be an ACL? maybe better to just be a thread:muse M:N.
	current_muse uuid,
	tag_pass_mode tinyint -- round robin? free for all?
);
create table posts (
	post_id uuid primary key,
	thread uuid, -- nil for starter post
	muse uuid,
	avatar uuid,
	body clob, -- text?
	style tinyint -- enum: normal, speechbubble
);
create table chats (
	chat_id uuid,
	visibiliy_mode tinyint -- enum: anchor readers can see it, anchor writers can see it
	-- choose exactly one. ACLs inherited from anchor.
	comm_anchor uuid, -- membership
	post_anchor uuid, -- write permission in that post's context
	muse_anchor uuid -- interaction permissions
);
create table direct_messages (
	dm_id uuid,
	initiator uuid,
	recipient uuid,
	subject varchar(200)
);
create table messages (
	message_id uuid,
	chat uuid,
	mun uuid references muns(mun),
	userpic uuid,
	message clob
);
create table page (
	page_id uuid,
	-- choose exactly one. read ACLs inherited from anchor.
	comm_anchor uuid,
	muse_anchor uuid,
	mun_anchor uuid references muns(mun),
	content clob
);
-- If the ACL's mode is allow, then allowed entries are allowed, else denied entries are denied, else allow.
-- If the ACL's mode is deny, then denied entries are denied, else allowed entries are allowed, else deny.


create table acl (
	acl_id uuid,
	owner uuid references muns(mun), --mun
	acl_mode tinyint -- enum: default allow, default deny
);
create table mun_acl (
	acl uuid,
	mun uuid references muns(mun),
	acl_mode tinyint -- enum: allow, deny
);
create table muse_acl (
	acl uuid,
	muse uuid,
	acl_mode tinyint -- enum: allow, deny
);
-- comm stuff definitely not needed yet
--create table comm (
--	comm_id uuid,
--	read_acl uuid,
--	shortname varchar(20), -- in URLs
--	name varchar(100),
--	icon char(43), -- sha256 base64
--	-- may decide in the future to make comm membership/adminship ACL-based but seems unnecessary?
--);


create table avatars (
	avatar_id uuid,
	muse uuid,
	position int,
	option_text varchar(40),
	tooltip_text varchar(140),
	fallback_text varchar(140),
	credit_text varchar(140)
);
create table userpics (
	userpic_id uuid,
	mun uuid references muns(mun),
	position int,
	option_text varchar(40),
	tooltip_text varchar(140),
	fallback_text varchar(140),
	credit_text varchar(140)
);


