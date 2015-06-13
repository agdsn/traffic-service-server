CREATE TABLE inbound (
	ip_dst VARCHAR NOT NULL,
	packets UNSIGNED INTEGER DEFAULT 0,
	bytes UNSIGNED BIG INTEGER DEFAULT 0,
	stamp_inserted INTGER NOT NULL,
	stamp_updated INTEGER,
	CONSTRAINT inbound_pk PRIMARY KEY (ip_dst, stamp_inserted)
);

CREATE TABLE outbound (
	ip_src VARCHAR NOT NULL,
	packets UNSIGNED INTEGER DEFAULT 0,
	bytes UNSIGNED BIG INTEGER DEFAULT 0,
	stamp_inserted INTGER NOT NULL,
	stamp_updated INTEGER,
	CONSTRAINT outbound_pk PRIMARY KEY (ip_src, stamp_inserted)
);
