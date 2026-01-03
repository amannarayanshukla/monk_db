# MonkDB Roadmap

## Phase 1: In-Memory Engine (Volatile)
### Epic 1: Core Data Structures
- [ ] **Story 1.1: The Warmup (HashMap)**
    - [ ] Setup CMake and GTest infrastructure.
    - [ ] Implement basic Put/Get wrapper.
    - [ ] Benchmark vs std::unordered_map.
- [ ] **Story 1.2: The MemTable (Skip List)**
    - [ ] Define Node structure (flexible height).
    - [ ] Implement `Put` (Insertion with random height).
    - [ ] Implement `Get` (Search).
    - [ ] Implement `Delete`.
    - [ ] Implement Iterator (Start, Next, Valid, Key, Value).

---

## Phase 2: Durability (Crash-Safe)
### Epic 2: Write Ahead Log (WAL)
- [ ] **Story 2.1: Data Serialization**
    - [ ] Implement `EncodeVarint32`.
    - [ ] Implement `DecodeVarint32`.
- [ ] **Story 2.2: The Append-Only Log**
    - [ ] Define Log Record Format (CRC|Len|Type|Key|Val).
    - [ ] Implement `WALWriter::Append` with `fsync`.
- [ ] **Story 2.3: Crash Recovery**
    - [ ] Implement `WALReader` (iterate over log file).
    - [ ] Replay valid logs into SkipList on startup.

---

## Phase 3: Storage & Maintenance (Persistent)
### Epic 3: Storage Engine
- [ ] **Story 3.1: Memory Management**
    - [ ] Implement Arena/Slab allocator (optional optimization).
- [ ] **Story 3.2: SSTables (Sorted String Tables)**
    - [ ] Flush MemTable to disk (SSTable format).
    - [ ] Implement External Merge Sort (Compaction).
- [ ] **Story 3.3: Fast I/O**
    - [ ] Implement `mmap` reader for SSTables.

---

## Phase 4: Read Optimization (Fast)
### Epic 4: Caching & Filtering
- [ ] **Story 4.1: Block Cache**
    - [ ] Implement Thread-Safe LRU Cache.
    - [ ] Integrate Cache into Get() path.
- [ ] **Story 4.2: Bloom Filter**
    - [ ] Implement MurmurHash.
    - [ ] Build Bloom Filter on SSTable flush.
    - [ ] Check Bloom Filter before disk read.

---

## Phase 5: Observability & Production
### Epic 5: Metrics & Benchmarking
- [ ] **Story 5.1: Structured Logging**
    - [ ] Implement JSON Logger.
- [ ] **Story 5.2: Latency Tracking**
    - [ ] Implement P95/P99 Histogram.
- [ ] **Story 5.3: The Final Exam**
    - [ ] Load Generator (Write/Read mix).
    - [ ] Final Benchmark Report.

## Out of Scope (Future Work)
- Consistent Hashing(required for clustering, part of distributed architecture)

- Transactions
- Replication
- Distributed Architecture
- Advanced Querying (Secondary Indexes, Range Queries)
- Multi-Language Bindings (Python, Java, etc.)
- Cloud-Native Features (Kubernetes Operators, Cloud Storage Integration)
- Backup and Restore Mechanisms
- Security Features (Encryption at Rest, TLS)
- Advanced Compaction Strategies (Tiered, Leveled)
- Pluggable Storage Engines
- Time-Series Data Support
- Graph Data Support
- Machine Learning Integration
- User Management and Access Control
- Monitoring Integrations (Prometheus, Grafana)
- Automated Testing Frameworks (Fuzz Testing, Property-Based Testing)
- Continuous Integration/Continuous Deployment (CI/CD) Pipelines
- Documentation and Tutorials
- Community Building and Support Channels
- Commercial Support and Services
- Plugin System for Extensions
- Web-Based Management Console
- Mobile Client Libraries
- Data Migration Tools
- Schema Management
- Performance Profiling Tools
- Customizable Compaction and Flush Policies
- Real-Time Analytics
- Integration with Big Data Ecosystems (Hadoop, Spark)
- Support for Multiple Storage Backends (SSD, HDD, NVMe)
- Advanced Data Structures (e.g., HyperLogLog, Count-Min Sketch)
- Integration with Message Queues (Kafka, RabbitMQ)
- Support for Geospatial Data
- Data Versioning and Auditing
- Support for JSON/BSON Data Types
- Integration with Search Engines (Elasticsearch, Solr)
- Support for Multi-Tenancy
- Advanced Backup Strategies (Incremental, Differential)
- Support for Data Compression Algorithms (Zstd, LZ4)
- Integration with Configuration Management Tools (Ansible, Puppet)
- Support for Event-Driven Architectures (Webhooks, Serverless)
- Advanced Security Features (Role-Based Access Control, Auditing)
- Support for Hybrid Storage Models (In-Memory + Disk)
- Integration with Data Visualization Tools (Tableau, Power BI)
- Support for Custom Data Types and Serializers
- Integration with Cloud Providers (AWS, GCP, Azure)
- Support for Real-Time Data Streaming