--
-- Table structure for table `inbound`
--

DROP TABLE IF EXISTS `inbound`;
CREATE TABLE `inbound` (
  `ip_dst` char(15) NOT NULL,
  `packets` int(10) unsigned NOT NULL,
  `bytes` bigint(20) unsigned NOT NULL,
  `stamp_inserted` int(11) NOT NULL,
  `stamp_updated` int(11) DEFAULT NULL,
  PRIMARY KEY (`ip_dst`,`stamp_inserted`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Table structure for table `outbound`
--

DROP TABLE IF EXISTS `outbound`;
CREATE TABLE `outbound` (
  `ip_src` char(15) NOT NULL,
  `packets` int(10) unsigned NOT NULL,
  `bytes` bigint(20) unsigned NOT NULL,
  `stamp_inserted` int(11) NOT NULL,
  `stamp_updated` int(11) DEFAULT NULL,
  PRIMARY KEY (`ip_src`,`stamp_inserted`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
