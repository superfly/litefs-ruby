LiteFS Ruby Library
=================

This Ruby library is for interacting with LiteFS features that cannot be accessed
through the typical SQLite API.


## Halting

LiteFS provides the ability to halt writes on the primary node in order that
replicas may execute writes remotely and forward them back to the primary. This
isn't necessary in most usage, however, it can make running migrations simpler.

Write forwarding from the replica is much slower than executing the write
transaction directly on the primary so only use this for migrations or low-write
scenarios.

```ruby
// Execute a write transaction from any node.
// If this is a replica, it will run the inner function with the HALT lock.
user = LiteFS.with_halt do
  User.create(name: "David", occupation: "Code Artist")
end
```

`with_halt` accepts a single, optional, argument specifying the path to the sqlite3 database.  If not specified, it will default to the ActiveRecord database if ActiveRecord is used, otherwise to the value of the DATABASE_URL environment variable.