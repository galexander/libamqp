/*
   Copyright 2011-2012 StormMQ Limited

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include "Transport/Connection/ConnectionStateMachine.h"

DECLARE_TRANSITION_FUNCTION(initialized);

void amqp_connection_tls_initialize(amqp_connection_t *connection)
{
    transition_to_state(connection, initialized);
}

static void cleanup_resources(amqp_connection_t *connection)
{
}
void amqp_connection_tls_cleanup(amqp_connection_t *connection)
{
    cleanup_resources(connection);
}

int amqp_connection_tls_is_state(const amqp_connection_t *connection, const char *state_name)
{
    return connection->state.tls.name != 0 ? (strcmp(connection->state.tls.name, state_name) == 0) : false;
}

static void version_accepted(amqp_connection_t *connection)
{
    not_implemented(sasl_version_accepted);
}
static void version_rejected(amqp_connection_t *connection, uint32_t version)
{
    not_implemented(sasl_version_rejected);
}
static void start_while_initialized(amqp_connection_t *connection)
{
    connection->state.negotiator.start(connection, connection->specification_version.required.tls, version_accepted, version_rejected);
}
static void tunnel_accept_while_initialized(amqp_connection_t *connection, uint32_t requested_version)
{
    not_implemented(tunnel_accept_while_initialized);
}
DEFINE_TRANSITION_TO_STATE(initialized)
{
    connection->state.tls.connect = start_while_initialized;
    connection->state.tls.tunnel.accept = tunnel_accept_while_initialized;
}

/**********************************************
 Default states
 *********************************************/
static void illegal_state(amqp_connection_t *connection, const char *event)
{
    amqp_connection_error(connection, AMQP_ERROR_ILLEGAL_STATE,
            "Connection tls tunnel does not support \"%s\" while \"%s\" and connection is \"%s\".",
            event, connection->state.tls.name, connection->state.connection.name);
    amqp_fatal_program_error("Connection tls state error");
}

static void default_connect(amqp_connection_t *connection)
{
    illegal_state(connection, "Start");
}

static void default_tunnel_establish(amqp_connection_t *connection, uint32_t version)
{
    illegal_state(connection, "TunnelEstablish");
}

static void default_state_initialization(amqp_connection_t *connection, const char *state_name, void (*action_initializer)(amqp_connection_t *connection) LIBAMQP_TRACE_STATE_ARGS_DECL)
{
    save_old_state(connection->state.tls.name);

    connection->state.tls.connect = default_connect;
    connection->state.tls.tunnel.accept = default_tunnel_establish;

    connection->state.tls.name = state_name;

    action_initializer(connection);

    trace_state_transition(AMQP_TRACE_CONNECTION_TLS, connection->state.tls.name);
}
