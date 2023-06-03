class LiteFS
  # with_halt executes block with a HALT lock. This allows any node to perform writes
  # on the database file. If this is a replica node, it will forward those writes
  # back to the primary node. If this is the primary node, it will simply execute
  # fn since it does not need the HALT lock.
  #
  # Please note that this will stop all writes on the primary. It is slower than
  # processing a write directly on the primary since it requires two round trips
  # to acquire & release the lock.
  #
  # This function should only be used for periodic migrations or low-write
  # scenarios.
  def self.with_halt(filename = nil)
    # if filename is not passed, default to ActiveRecord's database or ENV['DATABASE_URL']
    if defined?(ActiveRecord) and ActiveRecord::Base.connection_db_config.adapter == "sqlite3"
      if defined? Rails
        filename ||= Rails.root.join(ActiveRecord::Base.connection_db_config.database).to_s
      else
        filename ||= File.expand_path(ActiveRecord::Base.connection_db_config.database)
      end
    elsif ENV['DATABASE_URL']
      uri = URI.parse(ENV['DATABASE_URL'])
      if %w(file sqlite3).include? uri.scheme
        filename ||= uri.path
      end
    end

    # if no filename, simply yield
    return yield unless filename

    # obtain lock, yield, unlock
    begin
      file = open("#{filename}-lock", "a+", 0666)

      rc = halt(file.fileno)
      throw new Error("flock rc = #{rc}") if rc != 0

      yield
    ensure
      if file
        rc = unhalt(file.fileno)
        throw new Error("funlock rc = #{rc}") if rc != 0

        file.close
      end
    end
  end
end

# load extension
require "litefs/litefs"
